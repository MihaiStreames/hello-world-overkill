TARGET := hello

AS  := nasm
LD  := ld.lld

ASFLAGS := -f elf64
LDFLAGS := -static --strip-all --build-id=none -T link.ld

all: $(TARGET)

hello.o: hello.s
	$(AS) $(ASFLAGS) -o $@ $<

$(TARGET): hello.o
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) hello.o

size: $(TARGET)
	@ls -lh $(TARGET) | awk '{print $$5, $$NF}'

.PHONY: all clean size
