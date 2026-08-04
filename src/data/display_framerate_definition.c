/* display_framerate_definition @ 0x8417EF30 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char display_framerate;

hs_global_external display_framerate_definition =
{
    "display_framerate",
    hs_type_boolean,
    { 0, 0 },
    &display_framerate,
    0
};
