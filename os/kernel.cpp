void printf (const char *str)
{
    unsigned short* VideoMemory = (unsigned short *)0xb80000;
    for (int i=0; str[i] != '\0'; ++i) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | (0x00FF & str[i]);
    }
}

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i!=&end_ctors; i++) {
        (*i)();
    }
}

extern "C" void kernelMain(void *multiboot_structure, unsigned int magicnumber)
{
    printf("Hello World! --- Jitish Kolanjery");
    while(1);
}