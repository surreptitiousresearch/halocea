/* first_person_camera_new @ 0x837C22C0 — reset first-person camera state (clears the cached FOV). */

#include "headers/first_person_camera.h"

void first_person_camera_new(first_person_camera *camera)
{
    camera->last_field_of_view = 0.0f;
}
