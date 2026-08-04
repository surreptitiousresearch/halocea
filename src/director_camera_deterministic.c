/* director_camera_deterministic @0x836E4FC8 — compute the deterministic camera position/orientation for
 * a unit, dispatching to the following-camera or first-person-camera solver based on the unit's desired
 * perspective. Returns the perspective flag (non-zero = third/following perspective). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern int16_t director_desired_perspective(int unit_index, int16_t *seat_state);
extern void following_camera_deterministic(int unit_index, real_point3d *position, real_vector3d *forward);
extern void first_person_camera_deterministic(int unit_index, real_point3d *position, real_vector3d *forward);

int director_camera_deterministic(int unit_index, real_point3d *position, real_vector3d *forward)
{
    __int16 perspective;
    int result = director_desired_perspective(unit_index, &perspective);
    if ((__int16)result)
        following_camera_deterministic(unit_index, position, forward);
    else
        first_person_camera_deterministic(unit_index, position, forward);
    return result;
}
