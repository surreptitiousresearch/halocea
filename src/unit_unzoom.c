/* unit_unzoom @0x836D1900 — cancel a unit's weapon zoom. If the unit is a local player whose current seat
 * is actually zoomed, play the zoomed weapon's unzoom sound (the [303] sound tag of the object in the unit's
 * current weapon/seat slot). Then clear both zoom level bytes (0xFF = not zoomed), zero the
 * integrated_night_vision_power float (doubles as zoom magnification here) and notify the player-control layer. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int player_index_from_unit_index(int unit_index);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);
extern void player_control_unzoom(int unit_index);

void unit_unzoom(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    int player_index = player_index_from_unit_index(unit_index);
    if ( player_index != -1
      && (unsigned __int16)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index != 0xFFFF
      && (unsigned __int8)unit->unit.current_zoom_level != 0xFF )  /* casts: header fields are signed */
    {
        int seat_weapon_index = unit->unit.current_weapon_index;
        if ( seat_weapon_index != -1 )
        {
            int weapon_object_index = unit->unit.weapon_object_indices[seat_weapon_index];
            if ( weapon_object_index != -1 )
            {
                unit_datum *weapon = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_object_index)->datum);
                int unzoom_sound = TAG_GET(weapon_definition, weapon->definition_index)->weapon.zoom_out_sound.index;   /* +1212 */
                if ( unzoom_sound != -1 )
                    unspatialized_impulse_sound_new(unzoom_sound, 1.0f);
            }
        }
    }

    unit->unit.current_zoom_level = 0xFF;
    unit->unit.desired_zoom_level = 0xFF;
    unit->unit.integrated_night_vision_power = 0.0f;   /* DB member name; cleared as the zoom magnification */
    player_control_unzoom(unit_index);
}
