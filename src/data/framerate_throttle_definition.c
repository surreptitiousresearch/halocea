/* framerate_throttle_definition @ 0x8417EF00 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char global_frame_rate_throttle;

hs_global_external framerate_throttle_definition =
{
    "framerate_throttle",
    hs_type_boolean,
    { 0, 0 },
    &global_frame_rate_throttle,
    0
};
