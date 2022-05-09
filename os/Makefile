.ONESHELL: # Executes as one shell. (So, cd will change directory)

GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o gdt.o kernel.o port.o  # interruptstubs.o interrupts.o kernel.o

# $@ means target file
# $< means input file

%.o: %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<

jkernel.bin: linker.ld $(objects)
	mkdir -p build/
	ld $(LDPARAMS) -T $< -o build/$@ $(objects)

install: jkernel.bin
	sudo cp $< /boot/jkernel.bin

copy: jkernel.iso
	sudo -S jitish cp $< /media/sf_VBoxSharedFolder/

jkernel.iso: jkernel.bin
		mkdir -p image/iso/boot/grub
		cd image
		cp ../build/$< iso/boot/
		echo 'set timeout=0' >> iso/boot/grub/grub.cfg
		echo 'set default=0' >> iso/boot/grub/grub.cfg
		echo '' >> iso/boot/grub/grub.cfg
		echo 'menuentry "J Operating System" {' >> iso/boot/grub/grub.cfg
		echo '    multiboot /boot/jkernel.bin' >> iso/boot/grub/grub.cfg
		echo '    boot' >> iso/boot/grub/grub.cfg
		echo '}' >> iso/boot/grub/grub.cfg
		# mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -quiet -o $@ "$IMAGE_DIR/"
		# mkisofs -o $@ "iso/"
		grub-mkrescue --output=$@ iso

.PHONY: clean

clean:
	rm -rf *.o *.bin *.iso iso/ image/*
