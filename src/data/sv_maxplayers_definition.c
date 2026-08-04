/* sv_maxplayers_definition @ 0x84180CD0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int sv_max_players_value;

hs_global_external sv_maxplayers_definition =
{
    "sv_maxplayers",
    hs_type_short_integer,
    { 0, 0 },
    &sv_max_players_value,
    21
};
