/* speed_hack_detection_definition @ 0x8417F9C0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char speed_hack_detection;

hs_global_external speed_hack_detection_definition =
{
    "speed_hack_detection",
    hs_type_boolean,
    { 0, 0 },
    &speed_hack_detection,
    0
};
