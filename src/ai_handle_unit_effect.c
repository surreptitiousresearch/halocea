/* ai_handle_unit_effect @0x836E93F8 — propagate a unit "effect" (an AI-significant event such as a death
 * cry, throttled per unit at +540/+544 with a 30-tick window and an effect-type priority) to the actor
 * system. For a vehicle (object type word at +180 == 1) the effect is dispatched to each biped child
 * occupant; for a biped (type 0) it goes straight to that unit's actor. No-op before AI is initialised. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/ai_globals.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern void actors_handle_unit_effect(int owner_unit_index, int16_t effect_type, int16_t volume);

void ai_handle_unit_effect(int owner_unit_index, int16_t effect_type, int16_t volume)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;
    if ( owner_unit_index == -1 || volume <= 0 )
        return;

    unit_datum *object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, owner_unit_index)->datum);
    int now = game_time_get();
    if ( effect_type > object_data->unit.last_unit_effect_type || now > object_data->unit.game_time_at_last_unit_effect + 30 )
    {
        int16_t object_type = object_data->object.type;
        object_data->unit.last_unit_effect_type = effect_type;
        object_data->unit.game_time_at_last_unit_effect = now;
        if ( object_type == object_type_vehicle )  /* vehicle: dispatch to biped occupants */
        {
            for ( int child = object_data->object.first_child_object_index; child != -1; )
            {
                unit_datum *child_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum);
                if ( !child_data->object.type )
                    actors_handle_unit_effect(child, effect_type, volume);
                child = child_data->object.next_object_index;
            }
        }
        else if ( !object_type )  /* biped */
        {
            actors_handle_unit_effect(owner_unit_index, effect_type, volume);
        }
    }
}
