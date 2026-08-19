#include <stdint.h>
/* network_game_server_send_message_to_all_machines @0x837565E0 — stub in this build (server-side network
 * messaging is compiled out); always reports failure (0). */

typedef struct network_game_server network_game_server;
typedef int network_message_type;

uint8_t network_game_server_send_message_to_all_machines(network_game_server *server, network_message_type type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority)
{
    return 0;
}
