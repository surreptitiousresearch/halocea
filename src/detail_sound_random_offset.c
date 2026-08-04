/* detail_sound_random_offset @ 0x837164D0 — pick a random spherical position offset for a looping
 * sound's detail one-shot, within the detail's distance/theta(yaw)/phi(pitch) bounds. A zero random
 * distance yields the zero vector; otherwise a random direction is built from the angle bounds and
 * scaled by the random distance. */

#include <stdint.h>
#include "headers/looping_sound_detail.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/blam_data_globals.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);

void detail_sound_random_offset(const looping_sound_detail *detail_definition, real_vector3d *offset)
{
    float distance = real_seed_random_range(get_global_local_random_seed_address(),
                                            detail_definition->distance_lower_bound,
                                            detail_definition->distance_upper_bound);

    if ( distance == 0.0f )
    {
        *offset = *global_zero_vector3d;
    }
    else
    {
        real_euler_angles2d angles;
        angles.__s1.pitch = real_seed_random_range(get_global_local_random_seed_address(),
                                              detail_definition->phi_lower_bound,
                                              detail_definition->phi_upper_bound);
        angles.__s1.yaw = real_seed_random_range(get_global_local_random_seed_address(),
                                            detail_definition->theta_lower_bound,
                                            detail_definition->theta_upper_bound);
        vector3d_from_euler_angles2d(offset, &angles);
        offset->n[0] = offset->n[0] * distance;
        offset->n[1] = offset->n[1] * distance;
        offset->n[2] = offset->n[2] * distance;
    }
}
