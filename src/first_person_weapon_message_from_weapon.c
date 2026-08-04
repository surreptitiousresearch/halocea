/* first_person_weapon_message_from_weapon @0x836A0308 — route a first-person weapon message (e.g. a reload
 * animation cue) to the first-person weapon that corresponds to a world weapon object. If the weapon has no
 * first-person representation (index -1), the message is instead played as a first-person weapon sound. */

#include <stdint.h>

extern int16_t first_person_weapon_index_from_weapon_index(int weapon_index);
extern void first_person_weapon_message(int16_t local_player_index, int16_t message_type);
extern void weapon_play_first_person_weapon_sound(int weapon_index, int16_t message_type);

void first_person_weapon_message_from_weapon(int weapon_index, int16_t message_type)
{
    int16_t first_person_weapon_index = first_person_weapon_index_from_weapon_index(weapon_index);
    first_person_weapon_message(first_person_weapon_index, message_type);
    if ( first_person_weapon_index == -1 )
        weapon_play_first_person_weapon_sound(weapon_index, message_type);
}
