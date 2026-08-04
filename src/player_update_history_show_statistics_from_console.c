/* player_update_history_show_statistics_from_console @0x837ABD5C — console command hook that dumps
 * the local update-history statistics. Reduces to fetching the active network game client.
 *
 * DEVIATION: the decompiler renders the body as two calls to global_network_game_client_get() (a
 * tail-call/inline artifact); functionally it returns the client, or null when none is active. */

typedef struct network_game_client network_game_client;
extern network_game_client *global_network_game_client_get(void);

network_game_client *player_update_history_show_statistics_from_console(void)
{
    network_game_client *client = global_network_game_client_get();
    if ( client )
        return global_network_game_client_get();
    return client;
}
