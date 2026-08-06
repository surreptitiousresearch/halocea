/* log_server_player_update_history_definition @ 0x8417F970 (.data, 16 bytes)
 * DB applied_types: hs_global_external log_server_player_update_history_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120F9C -> "log_server_player_update_history"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84464484 -> log_server_player_update_history
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t log_server_player_update_history;

hs_global_external log_server_player_update_history_definition =
{
    "log_server_player_update_history",  /* name */
    hs_type_boolean,                     /* type */
    { 0, 0 },                            /* _pad06.._pad07 */
    &log_server_player_update_history,   /* pointer */
    0,                                   /* console_flags */
};
