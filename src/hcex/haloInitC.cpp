extern "C" int shell_initialize(void);

extern "C" int optionNoSound;

int haloInitC(void)
{
    optionNoSound = 0;
    shell_initialize();
    return 1;
}
