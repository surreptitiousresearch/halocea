/* first_person_weapon_message_from_unit @0x836A02AC — route a first-person weapon HUD/animation message
 * for the weapon a unit is wielding. When the unit has no first-person weapon instance, fall back to
 * playing the message sound directly on the unit's current weapon object. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern int16_t first_person_weapon_index_from_unit_index(int unit_index);
extern void first_person_weapon_message(int16_t local_player_index, int16_t message_type);
extern void weapon_play_first_person_weapon_sound(int weapon_index, int16_t message_type);

void first_person_weapon_message_from_unit(int unit_index, int16_t message_type)
{
    /* (int16_t) models extsh r11,r29 @0x836A02B4 — narrowed before the -1 compare */
    int16_t first_person_weapon_index = first_person_weapon_index_from_unit_index(unit_index);
    first_person_weapon_message(first_person_weapon_index, message_type);
    if ( first_person_weapon_index == -1 )
    {
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        int16_t weapon_index = unit->unit.current_weapon_index;
        if ( weapon_index != -1 )
            weapon_play_first_person_weapon_sound(weapon_index, message_type);
    }
}
