/* player_magnetism_definition @ 0x84180B10 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char player_magnetism_flag;

hs_global_external player_magnetism_definition =
{
    "player_magnetism",
    hs_type_boolean,
    { 0, 0 },
    &player_magnetism_flag,
    0
};
