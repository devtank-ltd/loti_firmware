#Toolchain settings
TOOLCHAIN := arm-none-eabi

CC = $(TOOLCHAIN)-gcc
OBJCOPY = $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump
SIZE = $(TOOLCHAIN)-size

#Target CPU options
CPU_DEFINES = -mthumb -mcpu=cortex-m0 -DSTM32F0 -pedantic

GIT_COMMITS := $(shell git rev-list --count HEAD)
GIT_COMMIT := $(shell git log -n 1 --format="%h-%f")

#Compiler options
CFLAGS		+= -Os -g -c -std=gnu99
CFLAGS		+= -Wall -Wextra -Werror
CFLAGS		+= -fstack-usage -Wstack-usage=100
CFLAGS		+= -MMD -MP
CFLAGS		+= -fno-common -ffunction-sections -fdata-sections
CFLAGS		+= $(CPU_DEFINES)
CFLAGS		+= -DGIT_VERSION=\"[$(GIT_COMMITS)]-$(GIT_COMMIT)\"

INCLUDE_PATHS += -Ilibs/libopencm3/include -I.

LINK_SCRIPT = stm32f07xzb.ld

LINK_FLAGS =  -Llibs/libopencm3/lib --static -nostartfiles
LINK_FLAGS += -Llibs/libopencm3/lib/stm32/f0
LINK_FLAGS += -T$(LINK_SCRIPT) -lopencm3_stm32f0
LINK_FLAGS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group -Wl,--gc-sections
LINK_FLAGS += $(CPU_DEFINES)

SOURCES += main.c \
           log.c \
           uarts.c \
           uart_rings.c \
           cmd.c \
           usb_uarts.c \
           adcs.c \
           pulsecount.c \
           timers.c \
           io.c \
           ring.c

BUILD_DIR := build/
PROJECT_NAME := firmware

OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)%.o)
DEPS = $(SOURCES:%.c=$(BUILD_DIR)%.d)
TARGET_ELF = $(BUILD_DIR)$(PROJECT_NAME).elf
TARGET_HEX = $(TARGET_ELF:%.elf=%.hex)
TARGET_BIN = $(TARGET_ELF:%.elf=%.bin)
TARGET_DFU = $(TARGET_ELF:%.elf=%.dfu)


LIBOPENCM3 := libs/libopencm3/lib/libopencm3_stm32f0.a


default: $(TARGET_ELF)

$(BUILD_DIR).git.$(GIT_COMMIT):
	mkdir -p `dirname $@`
	rm -f $(BUILD_DIR).git.*
	touch $@

$(BUILD_DIR)main.o : $(LIBOPENCM3)

$(TARGET_ELF): $(LIBS) $(OBJECTS) $(LINK_SCRIPT)
	$(CC) $(OBJECTS) $(LINK_FLAGS) -o $(TARGET_ELF)

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

$(OBJECTS): $(BUILD_DIR)%.o: %.c $(BUILD_DIR).git.$(GIT_COMMIT)
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) $< -o $@

$(LIBOPENCM3) :
	$(MAKE) -C libs/libopencm3 TARGETS=stm32/f0

size: $(TARGET_ELF)
	$(SIZE) $(TARGET_ELF)

flash: $(TARGET_ELF)

	openocd -f interface/stlink-v2-1.cfg \
		    -f target/stm32f0x.cfg \
		    -c "init" -c "reset init" \
		    -c "flash write_image erase $(TARGET_ELF)" \
		    -c "reset" \
		    -c "shutdown"

$(TARGET_DFU): $(TARGET_BIN)
	cp $(TARGET_BIN) $(TARGET_DFU)
	dfu-suffix -a $(TARGET_DFU)

dfu : $(TARGET_DFU)
	dfu-util -D $(TARGET_DFU) -a 0 -s 0x08000000:leave


clean:
	rm -rf $(BUILD_DIR)


cppcheck:
	cppcheck --enable=all --std=c99 $(SOURCES)


debug_mon: $(TARGET_ELF)
	openocd -f board/st_nucleo_f0.cfg -f interface/stlink-v2-1.cfg -c "init" -c "reset init"

debug_gdb: $(TARGET_ELF)
	$(TOOLCHAIN)-gdb -ex "target remote localhost:3333" -ex "monitor reset halt" -ex load $(TARGET_ELF);

debug:
	screen $$(ls /dev/serial/by-id/usb-STMicroelectronics_STM32_STLink* -1 | head -n 1) 115200 8n1

cmd:
	screen $$(ls /dev/serial/by-id/usb-Devtank_Ltd_IO_Board_Prototype-if00* -1 | head -n 1) 115200 8n1

desktop_boot:
	sudo gpioset --mode=signal --background 0 3=0
	sudo gpioset -m time -s 1 0 2=0
	sudo gpioset -m time -s 1 0 2=1
	sudo pkill gpioset

desktop_reset:
	sudo gpioset -m time -s 1 0 2=0
	sudo gpioset -m time -s 1 0 2=1

desktop_dfu:
	sudo gpioset --mode=signal --background 0 3=1
	sudo gpioset -m time -s 1 0 2=0
	sudo gpioset -m time -s 1 0 2=1
	sudo pkill gpioset


$(BUILD_DIR)stack_info : $(TARGET_ELF)
	./avstack.pl $(OBJECTS) > $@

stack_info: $(BUILD_DIR)stack_info
	cat $(BUILD_DIR)stack_info

-include $(DEPS)
