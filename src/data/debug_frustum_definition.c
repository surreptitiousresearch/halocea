/* debug_frustum_definition @ 0x8417F630 (.data, 16 bytes)
 * DB applied_types: hs_global_external debug_frustum_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821214E4 -> "debug_frustum"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84469589 -> render_camera_debug_this_fucking_frustum
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char render_camera_debug_this_fucking_frustum;

hs_global_external debug_frustum_definition =
{
    "debug_frustum",                            /* name */
    hs_type_boolean,                            /* type */
    { 0, 0 },                                   /* _pad06.._pad07 */
    &render_camera_debug_this_fucking_frustum,  /* pointer */
    0,                                          /* console_flags */
};
