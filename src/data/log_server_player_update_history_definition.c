/* log_server_player_update_history_definition @ 0x8417F970 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned __int8 log_server_player_update_history;

hs_global_external log_server_player_update_history_definition =
{
    "log_server_player_update_history",
    hs_type_boolean,
    { 0, 0 },
    &log_server_player_update_history,
    0
};
