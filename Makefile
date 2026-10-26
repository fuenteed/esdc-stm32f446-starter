# =============================================================================
#  OPTIONAL: build without PlatformIO (plain arm-none-eabi-gcc + make)
# =============================================================================
#  Most people should use PlatformIO (see README). This Makefile is for anyone
#  who wants to see the raw compiler commands, or already has the ARM GNU
#  toolchain installed.
#
#    make                  # build challenge 0
#    make CHALLENGE=1      # build challenge 1
#    make CHALLENGE=1 flash   # build + flash with OpenOCD
#    make clean
#
#  CHALLENGE can be 0, 1, 2, 3, 4 or playground.
# =============================================================================

CHALLENGE ?= 0

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

BUILD   = build/challenge$(CHALLENGE)
ifeq ($(CHALLENGE),playground)
  BUILD = build/playground
  CHALLENGE_SRC = src/challenges/playground.c
else
  CHALLENGE_SRC = $(wildcard src/challenges/challenge$(CHALLENGE)_*.c)
endif

ifeq ($(CHALLENGE_SRC),)
  $(error No file for CHALLENGE=$(CHALLENGE). Use 0, 1, 2, 3, 4 or playground)
endif

SRCS = src/startup_stm32f446.c src/esdc_helpers.c $(CHALLENGE_SRC)
OBJS = $(patsubst %.c,$(BUILD)/%.o,$(SRCS))

CPU     = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS  = $(CPU) -std=gnu11 -Os -g3 -Wall -Wextra -Wno-override-init \
          -ffunction-sections -fdata-sections -Iinclude
# -nostartfiles: our own startup file replaces the C library's.
LDFLAGS = $(CPU) -T linker/stm32f446re.ld -nostartfiles -Wl,--gc-sections \
          -Wl,-Map=$(BUILD)/firmware.map --specs=nano.specs
LDLIBS  = -lc -lgcc -lnosys

all: $(BUILD)/firmware.elf $(BUILD)/firmware.bin
	@$(SIZE) $(BUILD)/firmware.elf

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/firmware.elf: $(OBJS) linker/stm32f446re.ld
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(OBJCOPY) -O binary $< $@

flash: $(BUILD)/firmware.elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	        -c "program $< verify reset exit"

clean:
	rm -rf build

.PHONY: all flash clean
