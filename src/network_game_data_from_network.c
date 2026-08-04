/* network_game_data_from_network @0x83782BD0 — DEVIATION: disasm shows `client` (r4) is never read — the
 * function only touches r3 (`header`), and game_connection()'s return value is discarded (r3 is
 * immediately overwritten with the saved `header` before the tail call). game_connection() is called for
 * its side effect only; `client` is a genuinely dead passthrough, matching the DB's own 2-argument
 * signature, so no phantom use was invented. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"

extern int16_t game_connection(void);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);

int network_game_data_from_network(message_delta_processor_header *const header, network_game_client *client)
{
    game_connection();
    return message_delta_processor_discard_iteration_body(header);
}
