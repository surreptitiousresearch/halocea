/* debug_sound_channels_definition @ 0x8417F800 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char debug_sound_channels;

hs_global_external debug_sound_channels_definition =
{
    "debug_sound_channels",
    hs_type_boolean,
    { 0, 0 },
    &debug_sound_channels,
    0
};
