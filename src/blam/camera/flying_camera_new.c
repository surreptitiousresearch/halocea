/* flying_camera_new @0x837C2D80 — resets a flying_camera to the origin, level orientation, and a 70-degree
 * (1.2217305 rad) field of view. */

#include "headers/flying_camera.h"

void flying_camera_new(flying_camera *camera)
{
    camera->position.n[0] = 0.0f;
    camera->position.n[1] = 0.0f;
    camera->orientation.n[0] = 0.0f;
    camera->orientation.n[1] = 0.0f;
    camera->roll = 0.0f;
    camera->field_of_view = 1.2217305f;
}
