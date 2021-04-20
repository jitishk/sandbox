
GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o

%.o: %.cpp
	g++ $(GPPPARAMS) -o $@ -c $<

%.o: *.s
	as $(ASPARAMS) -o $@ $<

jkernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: jkernel.bin
	sudo cp $< /boot/jkernel.bin

jkernel.iso: jkernel.bin
		mkdir -p iso/boot/grub
		cp $< iso/boot/
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

clean:
	rm -rf *.o *.bin *.iso iso/


