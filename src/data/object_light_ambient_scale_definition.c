/* object_light_ambient_scale_definition @ 0x8417F870 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern float object_light_ambient_scale;

hs_global_external object_light_ambient_scale_definition =
{
    "object_light_ambient_scale",
    hs_type_real,
    { 0, 0 },
    &object_light_ambient_scale,
    0
};
