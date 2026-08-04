/* remote_player_action_update_rate_definition @ 0x8417F8F0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int remote_player_action_update_rate;

hs_global_external remote_player_action_update_rate_definition =
{
    "remote_player_action_update_rate",
    hs_type_long_integer,
    { 0, 0 },
    &remote_player_action_update_rate,
    0
};
