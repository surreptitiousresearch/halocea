/* player_update_history_show_statistics_from_console @0x837ABD5C — console command hook that dumps
 * the local update-history statistics. Reduces to fetching the active network game client.
 *
 * DEVIATION: the decompiler renders the body as two calls to global_network_game_client_get() (the
 * statistics dump itself is compiled out of the retail build) and threads the second call's r3 to the
 * epilogue. The sole caller (the HaloScript evaluate wrapper @0x8372D21C) ignores r3 and no path writes
 * r3 explicitly, so the declared return is void. */

typedef struct network_game_client network_game_client;
extern network_game_client *global_network_game_client_get(void);

void player_update_history_show_statistics_from_console(void)
{
    if ( global_network_game_client_get() )
        global_network_game_client_get();
}
