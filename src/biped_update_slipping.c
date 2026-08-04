/* biped_update_slipping @0x837B2ABC — if the biped is in a "past landing recovery" state (state > 3,
 * see biped_datum.h) and its ground velocity (+104..+112, see biped_jump.c) exceeds ~1/30 units/tick,
 * emit both slipping footstep sounds (unless running a simulated update). `animation` is unused by this
 * function despite being a parameter (matches the compiled prototype). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/material_effect_event.h"
#include "headers/blam_data_globals.h"

extern void biped_make_footstep(int biped_index, int16_t event_index, int16_t contact_point_index);
typedef struct unit_animation_update_data unit_animation_update_data;

void biped_update_slipping(int biped_index, unit_animation_update_data *animation)
{
    biped_datum *biped =
        (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    if ( biped->biped.slipping_ticks <= 3 )
        return;

    float velocity_x = biped->object.translational_velocity.n[0];
    float velocity_y = biped->object.translational_velocity.n[1];
    float velocity_z = biped->object.translational_velocity.n[2];
    float speed_squared = velocity_x * velocity_x + velocity_y * velocity_y + velocity_z * velocity_z;

    if ( speed_squared <= 0.0011111111f || g_running_simulated_update )
        return;

    biped_make_footstep(biped_index, _material_effect_biped_sliding, 0);
    biped_make_footstep(biped_index, _material_effect_biped_sliding, 1);
}
