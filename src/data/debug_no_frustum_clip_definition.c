/* debug_no_frustum_clip_definition @ 0x8417F620 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned __int8 debug_no_frustum_clip;

hs_global_external debug_no_frustum_clip_definition =
{
    "debug_no_frustum_clip",
    hs_type_boolean,
    { 0, 0 },
    &debug_no_frustum_clip,
    0
};
