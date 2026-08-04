/* framerate_lock_definition @ 0x8417EF10 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned __int8 debug_force_frame_rate_update;

hs_global_external framerate_lock_definition =
{
    "framerate_lock",
    hs_type_boolean,
    { 0, 0 },
    &debug_force_frame_rate_update,
    0
};
