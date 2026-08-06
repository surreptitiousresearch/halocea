/* debug_no_frustum_clip_definition @ 0x8417F620 (.data, 16 bytes)
 * DB applied_types: hs_global_external debug_no_frustum_clip_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821214F4 -> "debug_no_frustum_clip"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84469588 -> debug_no_frustum_clip
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t debug_no_frustum_clip;

hs_global_external debug_no_frustum_clip_definition =
{
    "debug_no_frustum_clip",  /* name */
    hs_type_boolean,          /* type */
    { 0, 0 },                 /* _pad06.._pad07 */
    &debug_no_frustum_clip,   /* pointer */
    0,                        /* console_flags */
};
