/* following_camera_new @ 0x837C26E0 — reset third-person follow camera to its default (no unit/seat,
 * unit-scale distance, zero facing offset). */

#include "headers/following_camera.h"

void following_camera_new(following_camera *camera)
{
    camera->initialized = 0;
    camera->confined = 0;
    camera->crouched = 0;
    camera->zoomed = 0;
    camera->facing_offset.n[0] = 0.0f;
    camera->facing_offset.n[1] = 0.0f;
    camera->zoom_level = 0;
    camera->distance_scale = 1.0f;
    camera->unit_index = -1;
    camera->seat_index = -1;
}
