/* console_func_sleep @0x8376DBC0 — thunk: block the calling thread for the given number of milliseconds. */

extern void Sleep(unsigned int milliseconds);

void console_func_sleep(unsigned int milliseconds)
{
    Sleep(milliseconds);
}
