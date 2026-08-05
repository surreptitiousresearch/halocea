/* network_game_data_from_network @0x83782BD0 — DEVIATION: disasm shows `client` (r4) is never read — the
 * function only touches r3 (`header`), and game_connection()'s return value is discarded (r3 is
 * immediately overwritten with the saved `header` before the tail call). game_connection() is called for
 * its side effect only; `client` is a genuinely dead passthrough, matching the DB's own 2-argument
 * signature, so no phantom use was invented.
 * Return: the tail `bl` at 83782BEC leaves message_delta_processor_discard_iteration_body's r3 in
 * place, but the function computes no r3 of its own and has no in-corpus consumer, so the value is
 * inherited, not produced — attested void, matching the rest of the `*_from_network` handler family. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"

extern int16_t game_connection(void);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);

void network_game_data_from_network(message_delta_processor_header *const header, network_game_client *client)
{
    game_connection();
    message_delta_processor_discard_iteration_body(header);
}
