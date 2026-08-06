/* update_funcs @ 0x84184C90 (.data, 8 bytes)
 * DB applied_types: void (__fastcall *update_funcs[2])(flying_camera *, const camera_control *, observer_command *);
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x837C3F50 -> editor_camera_flying_update
 *   +0x0004 = 0x837C4250 -> editor_camera_orbiting_update
 */
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
