CC = gcc
CXX = g++
AS = nasm

CFLAGS = -ffreestanding -m64 -nostdlib

CXXFLAGS = -ffreestanding -m64 -nostdlib \
-fno-exceptions -fno-rtti \
-mno-red-zone \
-Wall -Wextra -Wpedantic \
-std=c++20 \
-Ilimine

BUILD_DIR = build
SRC_DIR = src

CPP_SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
ASM_SOURCES := $(shell find $(SRC_DIR) -name "*.asm")

CPP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))
ASM_OBJECTS := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

OBJECTS := $(CPP_OBJECTS) $(ASM_OBJECTS)

all: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS)
	ld -m elf_x86_64 -T linker.ld -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(AS) -f elf64 $< -o $@

iso: all
	mkdir -p build/iso/boot
	mkdir -p build/iso/boot/limine

	cp build/kernel.bin build/iso/boot/
	cp limine.conf build/iso/boot/
	cp limine/limine-bios.sys build/iso/boot/limine/
	cp limine/limine-bios-cd.bin build/iso/boot/limine/
	cp limine/limine-uefi-cd.bin build/iso/boot/limine/

	xorriso -as mkisofs \
	-b boot/limine/limine-bios-cd.bin \
	-no-emul-boot \
	-boot-load-size 4 \
	-boot-info-table \
	--efi-boot boot/limine/limine-uefi-cd.bin \
	-efi-boot-part \
	--efi-boot-image \
	--protective-msdos-label \
	build/iso -o build/janelos.iso

	./limine/limine bios-install build/janelos.iso

run: iso
	qemu-system-x86_64 -cdrom build/janelos.iso

clean:
	rm -rf build

auto:
	$(MAKE) clean
	$(MAKE) iso
	$(MAKE) run

