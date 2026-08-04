/* local_player_vehicle_update_rate_definition @ 0x8417F8E0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned int local_player_vehicle_update_rate;

hs_global_external local_player_vehicle_update_rate_definition =
{
    "local_player_vehicle_update_rate",
    hs_type_long_integer,
    { 0, 0 },
    &local_player_vehicle_update_rate,
    0
};
