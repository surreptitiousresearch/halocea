/* sv_mapcycle_timeout_definition @ 0x84180CB0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int gMapCycleTimeoutSeconds;

hs_global_external sv_mapcycle_timeout_definition =
{
    "sv_mapcycle_timeout",
    hs_type_long_integer,
    { 0, 0 },
    &gMapCycleTimeoutSeconds,
    95
};
