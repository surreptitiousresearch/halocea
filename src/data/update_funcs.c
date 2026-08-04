#include "../headers/flying_camera.h"
#include "../headers/camera_control.h"
#include "../headers/observer_command.h"

/* .data init @0x84184C90: { &editor_camera_flying_update, &editor_camera_orbiting_update } */
extern void editor_camera_flying_update(flying_camera *camera, const camera_control *controls, observer_command *result);
extern void editor_camera_orbiting_update(flying_camera *camera, const camera_control *controls, observer_command *result);

void (*update_funcs[2])(flying_camera *, const camera_control *, observer_command *) =
{
    editor_camera_flying_update,
    editor_camera_orbiting_update,
};
