/* camera_script_globals @ 0x84177CA0 (.data, 64 bytes)
 * DB applied_types: $49197FBDBC1AD3D1BAA83ADB63B3EA44 camera_script_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 enabled                    = 0x00
 *   +0x01 first_update               = 0x00
 *   +0x02 mode                       = 0xFFFF
 *   +0x04 camera_point_index         = 0xFFFF
 *   +0x08 timer                      = 0x00000000 -> 0f
 *   +0x0C point                      = 0x000000000000000000000000
 *   +0x18 forward                    = 0x00000000000000003F800000
 *   +0x24 up                         = 0x000000003F80000000000000
 *   +0x30 field_of_view              = 0x3F9C61AA -> 1.22173f
 *   +0x34 relative_object_index      = 0xFFFFFFFF
 *   +0x38 animation_graph_index      = 0xFFFFFFFF
 *   +0x3C animation_index            = 0x0000
 * data-init camera_script_globals_t (64 bytes,
 * struct extent 62 + pad). Reconstructed from the binary (big-endian):
 *   mode=-1, camera_point_index=-1, forward=(0,0,1), up=(0,1,0),
 *   field_of_view=0x3F9C61AA (1.22173047 rad = 70 deg),
 *   relative_object_index=-1, animation_graph_index=-1; all else zero.
 */
#include "../headers/camera_script_globals.h"

camera_script_globals_t camera_script_globals =
{
    0,                    /* enabled */
    0,                    /* first_update */
    -1,                   /* mode */
    -1,                   /* camera_point_index */
    { 0, 0 },             /* _pad06 */
    0.0f,                 /* timer */
    { { 0.0f, 0.0f, 0.0f } }, /* point */
    { { 0.0f, 0.0f, 1.0f } }, /* forward */
    { { 0.0f, 1.0f, 0.0f } }, /* up */
    1.22173047f,          /* field_of_view (0x3F9C61AA) */
    -1,                   /* relative_object_index */
    -1,                   /* animation_graph_index */
    0                     /* animation_index */
};
