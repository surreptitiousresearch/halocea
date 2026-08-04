/* material_effect_visible @0x836EA840 — whether a material effect at `origin` is worth spawning: true
 * if more than two local players exist, or if any local player's camera is within 10 world units of the
 * origin (squared distance < 100). */

#include <stdint.h>
#include "headers/observer_result.h"
#include "headers/real_point3d.h"

extern int16_t local_player_count(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern const observer_result *observer_get_camera(int16_t local_player_index);

uint8_t material_effect_visible(const real_point3d *origin)
{
    __int16 local_player_index;

    if ( local_player_count() > 2 )
        return 1;

    for ( local_player_index = 0; local_player_index < 2; local_player_index = (__int16)(local_player_index + 1) )
    {
        if ( local_player_get_player_index(local_player_index) != -1 )
        {
            const observer_result *camera = observer_get_camera(local_player_index);
            float dx = origin->n[0] - camera->position.n[0];
            float dy = origin->n[1] - camera->position.n[1];
            float dz = origin->n[2] - camera->position.n[2];
            if ( ((dy * dy) + ((dx * dx) + (dz * dz))) < 100.0 )
                return 1;
        }
    }
    return 0;
}
