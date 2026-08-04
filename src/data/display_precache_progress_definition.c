/* display_precache_progress_definition @ 0x8417EF50 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char display_precache_progress;

hs_global_external display_precache_progress_definition =
{
    "display_precache_progress",
    hs_type_boolean,
    { 0, 0 },
    &display_precache_progress,
    0
};
