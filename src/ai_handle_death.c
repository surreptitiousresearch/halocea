/* ai_handle_death @0x836E8958 — notify the AI systems that a unit just died. Resolves the "responsible"
 * unit behind the killing object (a vehicle's driver/operator via actor fields [201]/[202], unless the
 * damage category is _damage_category_vehicle), classifies the relationship to the victim (self / ally / enemy),
 * and raises an AI communication event plus conversation/encounter death notifications.
 *
 * Deviation: the decompiler renders the enemy test as a _cntlzw bit-twiddle; it is simply
 * game_team_is_enemy(victim_team, killer_team) ? 3 : 2 (event subtype: 2 = ally kill, 3 = enemy kill). */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/object_type.h"
#include "headers/damage_category.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_data.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern void ai_conversation_unit_died(int unit_index, uint8_t deleted);
extern void encounters_unit_died(int unit_index);

void ai_handle_death(int unit_index, int owner_object_index, int16_t damage_category)
{
    int responsible_unit_index = -1;
    if ( owner_object_index != -1 )
    {
        int *actor = object_try_and_get_and_verify_type(owner_object_index, object_mask_unit);
        if ( actor )
        {
            /* vehicle-category damage or no controlling actor -> fall back to the operator */
            if ( damage_category == _damage_category_vehicle || (responsible_unit_index = ((unit_datum *)actor)->unit.gunner_object_index, responsible_unit_index == -1) )
            {
                responsible_unit_index = ((unit_datum *)actor)->unit.driver_object_index;
                if ( responsible_unit_index == -1 )
                    responsible_unit_index = owner_object_index;
            }
        }
    }

    int16_t event_subtype = _comm_hostility_none;
    if ( unit_index == responsible_unit_index )
    {
        event_subtype = _comm_hostility_self;
    }
    else if ( responsible_unit_index != -1 )
    {
        int16_t victim_team = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum)->object.owner_team_index;
        int16_t killer_team = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, responsible_unit_index)->datum)->object.owner_team_index;
        event_subtype = game_team_is_enemy(victim_team, killer_team) ? _comm_hostility_enemy : _comm_hostility_friend;
    }

    ai_communication_event(_ai_communication_death, unit_index, responsible_unit_index, event_subtype, damage_category, -1, 0);
    ai_conversation_unit_died(unit_index, 0);
    encounters_unit_died(unit_index);
}
