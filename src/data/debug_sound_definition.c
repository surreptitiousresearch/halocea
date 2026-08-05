/* debug_sound_definition @ 0x84180CE0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t debug_sound;

hs_global_external debug_sound_definition =
{
    "debug_sound",
    hs_type_boolean,
    { 0, 0 },
    &debug_sound,
    0
};
