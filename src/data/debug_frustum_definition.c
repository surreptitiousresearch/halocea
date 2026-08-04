/* debug_frustum_definition @ 0x8417F630 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char render_camera_debug_this_fucking_frustum;

hs_global_external debug_frustum_definition =
{
    "debug_frustum",
    hs_type_boolean,
    { 0, 0 },
    &render_camera_debug_this_fucking_frustum,
    0
};
