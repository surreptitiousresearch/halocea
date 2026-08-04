/* debug_framerate_definition @ 0x8417EF20 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char debug_frame_rate;

hs_global_external debug_framerate_definition =
{
    "debug_framerate",
    hs_type_boolean,
    { 0, 0 },
    &debug_frame_rate,
    0
};
