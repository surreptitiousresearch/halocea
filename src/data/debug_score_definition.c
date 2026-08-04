/* debug_score_definition @ 0x8417F9B0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int debug_score;

hs_global_external debug_score_definition =
{
    "debug_score",
    hs_type_long_integer,
    { 0, 0 },
    &debug_score,
    0
};
