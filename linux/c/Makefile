TARGET := hello

CC     := clang
AS     := nasm
LD     := ld.lld

CFLAGS  := --target=x86_64-pc-linux-gnu -Os -std=c11 -nostdlib -ffreestanding -ffunction-sections -fdata-sections
ASFLAGS := -f elf64
LDFLAGS := -static --gc-sections --strip-all --build-id=none -T link.ld

CSRC    := hello.c
COBJS   := $(CSRC:.c=.o)
ASM_OBJ := syscall.o

all: $(TARGET)

$(ASM_OBJ): syscall.s
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(COBJS) $(ASM_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(COBJS) $(ASM_OBJ)

size: $(TARGET)
	@ls -lh $(TARGET) | awk '{print $$5, $$NF}'

.PHONY: all clean size
