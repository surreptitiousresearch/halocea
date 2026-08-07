extern "C" int shell_initialize(void);

extern "C" int optionNoSound;

// DEVIATION: `extern "C"`. The binary exports this as the FLAT symbol `haloInitC` (@0x83682148);
// compiled as C++ without it the definition mangles to `?haloInitC@@YA...` and nothing links
// against it. 209 of the 219 flat-defining hcex TUs already do this -- these ten did not.
extern "C" int haloInitC(void)
{
    optionNoSound = 0;
    shell_initialize();
    return 1;
}
