/* player_spawn_count_definition @ 0x8417EEF0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern short player_spawn_count;

hs_global_external player_spawn_count_definition =
{
    "player_spawn_count",
    hs_type_short_integer,
    { 0, 0 },
    &player_spawn_count,
    0
};
