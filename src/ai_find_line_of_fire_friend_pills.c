/* ai_find_line_of_fire_friend_pills @0x836E7EE0 — collect the friendly collision volumes ("pills") an
 * actor must avoid shooting through, up to max_pill_count. Two sources:
 *   1. Every other actor in the same encounter that occupies a unit and is not in a vehicle: a pill is
 *      generated for that unit, tagged with the prop index returned by prop_get_active_by_unit_index.
 *   2. Every friendly prop (not enemy, not dead, engagement state 3, not in a vehicle, backing a real
 *      unit object) whose owning actor is NOT in our encounter (or has no actor / we have no encounter):
 *      a pill is generated for the prop's unit, tagged with the prop's own index.
 * Returns the number of pills written.
 *
 * DEVIATION: the second ai_generate_line_of_fire_pill call's prop_index arg shows as
 * `v15[0].encounter_index` in the decompiler because the stack slot was reused for a prop_iterator; the
 * disasm reads the prop_iterator's offset-0 field, i.e. the current prop index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/prop_iterator.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/line_of_fire_pill.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern int prop_get_active_by_unit_index(int actor_index, int unit_index);
extern void ai_generate_line_of_fire_pill(int unit_index, int prop_index, line_of_fire_pill *pill);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);

int16_t ai_find_line_of_fire_friend_pills(int actor_index, int16_t max_pill_count, line_of_fire_pill *pills)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t pill_count = 0;

    int encounter_index = actor->meta.encounter_index;
    if (encounter_index != -1)
    {
        encounter_actor_iterator iterator;
        encounter_actor_iterator_new(&iterator, encounter_index);
        for (actor_datum *other = encounter_actor_iterator_next(&iterator);
             other;
             other = encounter_actor_iterator_next(&iterator))
        {
            if (iterator.index != actor_index && pill_count < max_pill_count)
            {
                int unit_index = other->meta.unit_index;
                if (unit_index != -1 && other->input.vehicle_index == -1)
                {
                    int prop_index = prop_get_active_by_unit_index(actor_index, unit_index);
                    ai_generate_line_of_fire_pill(other->meta.unit_index, prop_index, &pills[pill_count]);
                    pill_count = (int16_t)(pill_count + 1);
                }
            }
        }
    }

    prop_iterator prop_cursor;
    prop_iterator_new(&prop_cursor, actor_index);
    for (prop_datum *prop = prop_iterator_next(&prop_cursor);
         prop;
         prop = prop_iterator_next(&prop_cursor))
    {
        if (!prop->enemy && !prop->dead && prop->state == _prop_state_acknowledged && prop->vehicle_index == -1)
        {
            object_datum *unit_object =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum;
            if (((1 << unit_object->object.type) & object_mask_biped) != 0)
            {
                int our_encounter_index = actor->meta.encounter_index;
                actor_datum *prop_owner = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
                if ((our_encounter_index == -1
                     || prop->actor_index == -1
                     || prop_owner->meta.encounter_index != our_encounter_index)
                    && pill_count < max_pill_count)
                {
                    ai_generate_line_of_fire_pill(prop->unit_index, prop_cursor.index, &pills[pill_count]);
                    pill_count = (int16_t)(pill_count + 1);
                }
            }
        }
    }

    return pill_count;
}
