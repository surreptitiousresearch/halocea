/* director_camera_switch_fast_definition @ 0x84180B50 (.data, 16 bytes)
 * DB applied_types: hs_global_external director_camera_switch_fast_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211F12C -> "director_camera_switch_fast"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8442A210 -> director_camera_switch_fast
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t director_camera_switch_fast;

hs_global_external director_camera_switch_fast_definition =
{
    "director_camera_switch_fast",  /* name */
    hs_type_boolean,                /* type */
    { 0, 0 },                       /* _pad06.._pad07 */
    &director_camera_switch_fast,   /* pointer */
    0,                              /* console_flags */
};
