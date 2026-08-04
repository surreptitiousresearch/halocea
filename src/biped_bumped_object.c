/* biped_bumped_object @0x837AE018 — track a biped's bump_ticks cooldown counter and, when the biped is
 * player/AI-controlled (or being played back by a recorded animation) and repeatedly bumps the same
 * object 4+ times, force a possession swap: if bump_possession is cheat-enabled and the biped has a
 * local player, take over the bumped object as that player's unit. bump_ticks also serves as a plain
 * cooldown (incremented while no object is being bumped; reset to -15 as a "just possessed something,
 * wait" cooldown floor) when the biped isn't otherwise eligible for the escalation logic. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/object_type.h"
#include "headers/biped_datum.h"
#include "headers/blam_data_globals.h"


extern void ai_handle_bump(int biped_index, int object_index, const real_vector3d *old_velocity);
extern uint8_t recorded_animation_controlling_unit(int unit_index);
extern int16_t unit_get_local_player_index(int unit_index);
extern void players_set_local_player_unit(int16_t local_player_index, int new_unit_index);

void biped_bumped_object(int biped_index, int object_index, const real_vector3d *old_velocity)
{
    biped_datum *biped = (biped_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    char bump_counter = biped->biped.bump_ticks;

    if ( bump_counter < 0 )
    {
        if ( object_index == -1 )
            biped->biped.bump_ticks = bump_counter + 1;
        else
            biped->biped.bump_ticks = -15;
        return;
    }

    if ( object_index == -1 )
        return;

    biped_datum *object = (biped_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    ai_handle_bump(biped_index, object_index, old_velocity);

    if ( biped->unit.player_index == -1 && !recorded_animation_controlling_unit(biped_index) )
        return;

    if ( biped->biped.bump_object_index != object_index )
    {
        biped->biped.bump_object_index = object_index;
        biped->biped.bump_ticks = 0;
        return;
    }

    char new_bump_counter = biped->biped.bump_ticks + 1;
    biped->biped.bump_ticks = new_bump_counter;

    if ( new_bump_counter <= 3 )
        return;

    if ( object->object.type == object_type_biped )  /* bumped object is itself a biped */
    {
        if ( cheat.bump_possession )
        {
            __int16 local_player_index = unit_get_local_player_index(biped_index);
            if ( local_player_index != -1 )
            {
                object->biped.bump_ticks = -15;
                players_set_local_player_unit(local_player_index, object_index);
            }
        }
    }

    biped->biped.bump_ticks = -15;
}
