BUILD := build
KDIR := kernel

TARGET := oldarmfw

TRIPLET := arm-none-eabi
CC := $(TRIPLET)-gcc
LD := $(TRIPLET)-ld
OC := $(TRIPLET)-objcopy

INC  := -Iinclude

BFLAGS := -DARM9 -marm -mno-thumb-interwork -mcpu=arm946e-s

ASFLAGS := $(BFLAGS) $(INC) -x assembler-with-cpp
CFLAGS  := $(BFLAGS) $(INC) -Wall -O2 -ffast-math -ffunction-sections \
			-fdata-sections -fomit-frame-pointer -ffreestanding
LDFLAGS := -Tlink.ld -lgcc -nostartfiles -ffreestanding -Wl,--nmagic

OBJS += $(patsubst $(KDIR)/%.c, $(BUILD)/%.c.o, \
		$(patsubst $(KDIR)/%.s, $(BUILD)/%.s.o, \
		$(shell find "$(KDIR)" -name '*.c' -o -name '*.s')))

.PHONY: all test clean $(TARGET).bin
all: $(TARGET).bin

test: $(TARGET).elf
	scp $^ wolfvak@wolfvak.ddns.net:~/$^
	ssh -X wolfvak@wolfvak.ddns.net "DISPLAY=192.168.1.2:0 qemu-ctr -M ctr9 -kernel $^"

clean:
	@rm -rf $(TARGET).elf $(TARGET).bin $(BUILD)

$(TARGET).bin: $(TARGET).elf
	$(OC) -O binary $^ $@

$(TARGET).elf: $(OBJS)
	@mkdir -p "$(@D)"
	$(CC) $^ $(LDFLAGS) -o $@

$(BUILD)/%.c.o: $(KDIR)/%.c
	@mkdir -p "$(@D)"
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD)/%.s.o: $(KDIR)/%.s
	@mkdir -p "$(@D)"
	$(CC) -c $(ASFLAGS) -o $@ $<
