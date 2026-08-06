/* should_play_multiplayer_hit_sound_definition @ 0x8417F9E0 (.data, 16 bytes)
 * DB applied_types: hs_global_external should_play_multiplayer_hit_sound_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120EF4 -> "should_play_multiplayer_hit_sound"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84175D18 -> should_play_multiplayer_hit_sound
 *   +0x0C console_flags              = 0x5F
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char should_play_multiplayer_hit_sound;

hs_global_external should_play_multiplayer_hit_sound_definition =
{
    "should_play_multiplayer_hit_sound",  /* name */
    hs_type_boolean,                      /* type */
    { 0, 0 },                             /* _pad06.._pad07 */
    &should_play_multiplayer_hit_sound,   /* pointer */
    95,                                   /* console_flags */
};
