/* object_prediction_definition @ 0x8417F710 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char object_prediction;

hs_global_external object_prediction_definition =
{
    "object_prediction",
    hs_type_boolean,
    { 0, 0 },
    &object_prediction,
    0
};
