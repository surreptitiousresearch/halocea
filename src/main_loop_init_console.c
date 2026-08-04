/* main_loop_init_console @0x83688C28 — start the developer console (thunk to console_startup). */

extern int console_startup(void);

int main_loop_init_console(void)
{
    return console_startup();
}
