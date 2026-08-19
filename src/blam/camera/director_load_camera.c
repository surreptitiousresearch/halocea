/* director_load_camera @0x836E4D10 — debug helper: read a camera (position, forward, up, field of view) back
 * from "d:\camera.txt" and install it as local player 0's flying camera. The stored up vector is compared
 * against the up derived from the forward vector to recover the camera roll (signed via a triple product),
 * then the flying-camera state, roll, FOV and update proc are set. */

#include <stdio.h>
#include "headers/director.h"
#include "headers/director_camera_mode.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/flying_camera.h"

extern void flying_camera_new_from_point_and_vector(flying_camera *camera, const real_point3d *focus, const real_vector3d *orientation);
extern void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern void flying_camera_update(flying_camera *camera, const camera_control *controls, observer_command *result);

void director_load_camera(void)
{
    FILE *file = fopen("d:\\camera.txt", "r");
    if ( !file )
        return;

    real_point3d position;
    real_vector3d forward;
    real_vector3d stored_up;
    real_vector3d derived_up;
    float field_of_view;

    fscanf(file, "%f %f %f\n", &position.n[0], &position.n[1], &position.n[2]);
    fscanf(file, "%f %f %f\n", &forward.n[0], &forward.n[1], &forward.n[2]);
    fscanf(file, "%f %f %f\n", &stored_up.n[0], &stored_up.n[1], &stored_up.n[2]);
    fscanf(file, "%f\n", &field_of_view);
    fclose(file);

    flying_camera *camera = (flying_camera *)director_globals.local_players[0].camera_data;
    flying_camera_new_from_point_and_vector(camera, &position, &forward);
    observer_up_from_forward(&forward, &derived_up);
    float roll = angle_between_vectors3d(&stored_up, &derived_up);

    /* Triple product (stored_up x derived_up) . forward to choose the roll sign. */
    float triple_product =
        ((derived_up.n[2] * stored_up.n[1]) - (stored_up.n[2] * derived_up.n[1])) * forward.n[0]
      + (forward.n[2] * ((stored_up.n[0] * derived_up.n[1]) - (derived_up.n[0] * stored_up.n[1]))
       + forward.n[1] * ((stored_up.n[2] * derived_up.n[0]) - (derived_up.n[2] * stored_up.n[0])));

    /* recovered: *(float *)&camera_data[20] -> camera->roll, [24] -> camera->field_of_view */
    camera->roll = roll;
    if ( triple_product > 0.0f )
        camera->roll = -roll;
    camera->field_of_view = field_of_view;

    director_globals.local_players[0].debug_controls = 0;
    director_globals.local_players[0].camera_proc = flying_camera_update;
    director_globals.local_players[0].debug_input_scale = 1.0f;
    director_globals.local_players[0].camera_mode_index = _camera_flying;
}
