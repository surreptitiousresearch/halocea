/* debug_looping_sound_definition @ 0x8417F7F0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char debug_looping_sound;

hs_global_external debug_looping_sound_definition =
{
    "debug_looping_sound",
    hs_type_boolean,
    { 0, 0 },
    &debug_looping_sound,
    0
};
