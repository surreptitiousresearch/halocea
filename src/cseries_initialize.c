extern void cseries_platform_initialize(void);

void cseries_initialize(void)
{
    /* tail-call thunk (b @ 0x836F5888); only caller shell_initialize ignores r3 -> void */
    cseries_platform_initialize();
}
