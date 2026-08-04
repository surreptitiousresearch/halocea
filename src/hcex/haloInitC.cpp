extern int shell_initialize(void);

extern int optionNoSound;

int haloInitC(void)
{
    optionNoSound = 0;
    shell_initialize();
    return 1;
}
