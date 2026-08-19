/* network_game_server_send_message_to_all_loaded_machines @0x837565E8 — stub. In this build the function
 * immediately returns 0 (no machines are served); the body is a single `li r3,0 / blr`. Kept as a translation
 * unit so callers (e.g. weapon_start_reload_to_network) link against a real definition. */

#include <stdint.h>

struct network_game_server;  /* pointee network_game_server: no DB layout to verify (param never dereferenced; stub) */

uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type,
                                                                void *message, int size_in_bits,
                                                                uint8_t reliable, uint8_t immediate,
                                                                uint8_t including_local_client, int priority)
{
    return 0;
}
