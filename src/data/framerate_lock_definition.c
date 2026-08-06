/* framerate_lock_definition @ 0x8417EF10 (.data, 16 bytes)
 * DB applied_types: hs_global_external framerate_lock_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821221B8 -> "framerate_lock"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8441DFF4 -> debug_force_frame_rate_update
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t debug_force_frame_rate_update;

hs_global_external framerate_lock_definition =
{
    "framerate_lock",                /* name */
    hs_type_boolean,                 /* type */
    { 0, 0 },                        /* _pad06.._pad07 */
    &debug_force_frame_rate_update,  /* pointer */
    0,                               /* console_flags */
};
