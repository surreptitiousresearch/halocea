/* model_animation_compression_definition @ 0x8417FA90 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char hs_model_animation_compression_enabled;

hs_global_external model_animation_compression_definition =
{
    "model_animation_compression",
    hs_type_boolean,
    { 0, 0 },
    &hs_model_animation_compression_enabled,
    0
};
