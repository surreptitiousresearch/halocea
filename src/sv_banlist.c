/* sv_banlist @0x83766858 — server console command thunk: print the current ban list to the console. */

extern int banlist_print_to_console(void);

int sv_banlist(void)
{
    return banlist_print_to_console();
}
