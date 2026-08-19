/* main_loop_init_console @0x83688C28 — start the developer console (one-instruction
 * `b console_startup` thunk).
 *
 * Deviation: attested void — the thunk's only in-binary caller (HALO_MNG::ProcessMsg @0x823DEAD4)
 * discards r3, so console_startup's status is not part of this interface. */

extern void console_startup(void);

void main_loop_init_console(void)
{
    console_startup();
}
