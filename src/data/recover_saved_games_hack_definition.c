/* recover_saved_games_hack_definition @ 0x8417F650 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char recover_saved_games_hack;

hs_global_external recover_saved_games_hack_definition =
{
    "recover_saved_games_hack",
    hs_type_boolean,
    { 0, 0 },
    &recover_saved_games_hack,
    0
};
