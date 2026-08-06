/* debug_looping_sound_definition @ 0x8417F7F0 (.data, 16 bytes)
 * DB applied_types: hs_global_external debug_looping_sound_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82121270 -> "debug_looping_sound"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x846DB70C -> debug_looping_sound
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char debug_looping_sound;

hs_global_external debug_looping_sound_definition =
{
    "debug_looping_sound",  /* name */
    hs_type_boolean,        /* type */
    { 0, 0 },               /* _pad06.._pad07 */
    &debug_looping_sound,   /* pointer */
    0,                      /* console_flags */
};
