/* debug_framerate_definition @ 0x8417EF20 (.data, 16 bytes)
 * DB applied_types: hs_global_external debug_framerate_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821221A8 -> "debug_framerate"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8441DFF7 -> debug_frame_rate
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char debug_frame_rate;

hs_global_external debug_framerate_definition =
{
    "debug_framerate",  /* name */
    hs_type_boolean,    /* type */
    { 0, 0 },           /* _pad06.._pad07 */
    &debug_frame_rate,  /* pointer */
    0,                  /* console_flags */
};
