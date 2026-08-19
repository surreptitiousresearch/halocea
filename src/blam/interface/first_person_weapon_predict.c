/* first_person_weapon_predict @0x8369EAE0 — precache the predicted resources for a local player's currently
 * held first-person weapon (the predicted_resources tag_block at weapon definition +1252), then arm the
 * 30-tick re-prediction timer. Skips the precache when no weapon is bound. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_block.h"
#include "headers/weapon_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/tag_block.h"
extern void predicted_resources_precache(const tag_block *predicted_resources);

void first_person_weapon_predict(int16_t local_player_index)
{
    first_person_weapon *fp_weapon = &first_person_weapons[local_player_index];

    if ( first_person_weapons[local_player_index].weapon_index != -1 )
    {
        weapon_definition *weapon_def = TAG_GET(weapon_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, first_person_weapons[local_player_index].weapon_index)->datum->definition_index);
        predicted_resources_precache(&weapon_def->weapon.predicted_resources);
    }

    fp_weapon->ticks_until_predict = 30;
}
