/* debug_sound_channels_definition @ 0x8417F800 (.data, 16 bytes)
 * DB applied_types: hs_global_external debug_sound_channels_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82121258 -> "debug_sound_channels"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x846DB704 -> debug_sound_channels
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char debug_sound_channels;

hs_global_external debug_sound_channels_definition =
{
    "debug_sound_channels",  /* name */
    hs_type_boolean,         /* type */
    { 0, 0 },                /* _pad06.._pad07 */
    &debug_sound_channels,   /* pointer */
    0,                       /* console_flags */
};
