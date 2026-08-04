/* camera_script_globals @ 0x84177CA0 — data-init camera_script_globals_t (64 bytes,
 * struct extent 62 + pad). Reconstructed from the binary (big-endian):
 *   mode=-1, camera_point_index=-1, forward=(0,0,1), up=(0,1,0),
 *   field_of_view=0x3F9C61AA (1.22173047 rad = 70 deg),
 *   relative_object_index=-1, animation_graph_index=-1; all else zero. */
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
