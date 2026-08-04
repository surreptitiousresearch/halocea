/* player_effect_continuous_refresh @0x83737708 — keep a continuous player effect (screen shake / camera
 * effect) alive for each local player, scaled by that player's distance from the effect origin.
 *
 * player datum (512-byte stride): unit object index dword[13] (@52). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern int local_player_get_player_index(int16_t local_player_index);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void player_effect_add_continuous_effect(int16_t local_player_index, int effect_index, float distance);

void player_effect_continuous_refresh(int effect_index, const real_point3d *origin)
{
    __int16 local_player_index;

    for ( local_player_index = 0; local_player_index < 2; local_player_index = (__int16)(local_player_index + 1) )
    {
        if ( local_player_get_player_index(local_player_index) != -1 )
        {
            int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                                         local_player_get_player_index(local_player_index))->unit_index;
            if ( unit_index != -1 )
            {
                real_point3d player_origin;
                float dx, dy, dz;
                object_get_origin(unit_index, &player_origin);
                dx = origin->n[0] - player_origin.n[0];
                dy = origin->n[1] - player_origin.n[1];
                dz = origin->n[2] - player_origin.n[2];
                player_effect_add_continuous_effect(local_player_index, effect_index,
                    __fsqrts(((dy * dy) + ((dx * dx) + (dz * dz)))));
            }
        }
    }
}
