/* network_game_server_send_message_to_machine @0x837565D0 — stub returning 0 (no message is sent). In this
 * build the single-machine server send is a `li r3,0 / blr`; kept as a real definition for linkage. */

#include <stdint.h>

struct network_game_server;  /* pointee network_game_server: no DB layout to verify (param never dereferenced; stub) */

uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index,
                                                    int type, void *message, int size_in_bits,
                                                    uint8_t reliable, uint8_t immediate,
                                                    uint8_t including_local_client, int priority)
{
    return 0;
}
