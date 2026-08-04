/* should_play_multiplayer_hit_sound_definition @ 0x8417F9E0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char should_play_multiplayer_hit_sound;

hs_global_external should_play_multiplayer_hit_sound_definition =
{
    "should_play_multiplayer_hit_sound",
    hs_type_boolean,
    { 0, 0 },
    &should_play_multiplayer_hit_sound,
    95
};
