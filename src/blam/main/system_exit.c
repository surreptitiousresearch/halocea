/* system_exit @0x83762AC0 — terminate by forcing a null-pointer write (the build's deliberate hard-stop;
 * `code` is ignored). The store to address 0 raises an access violation, halting the title. */

void system_exit(int code)
{
    *(int *)0 = 0;   /* MEMORY[0] = 0 — intentional fault to halt */
}
