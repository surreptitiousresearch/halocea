/* ai_scripting_renew @0x83771378 — "renew" every actor addressed by an AI index: mark its unit's body and
 * shield as freshly full (object.body_vitality/shield_vitality set to full when the corresponding
 * maximum_*_vitality is positive), and top its grenade inventory up to a random count in the actor variant's
 * grenade range. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/actor_variant_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern int unit_get_current_grenade_type(int unit_index);
extern int16_t unit_get_grenade_count(int unit_index, int16_t grenade_type);
extern int16_t unit_add_grenade_type_to_inventory(int unit_index, int16_t grenade_type, int16_t grenade_count);

void ai_scripting_renew(int ai_index)
{
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
          actor;
          actor = ai_index_actor_iterator_next(&iterator) )
    {
        if ( actor->meta.unit_index == -1 )
            continue;

        actor_variant_definition *variant_definition = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
        object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum;

        unit_object->object.body_vitality = unit_object->object.maximum_body_vitality > 0.0f ? 1.0f : 0.0f;     /* refill body to full */
        unit_object->object.shield_vitality = unit_object->object.maximum_shield_vitality > 0.0f ? 1.0f : 0.0f; /* refill shield to full */

        __int16 grenade_type = variant_definition->grenade_combat.grenade_type;           /* variant grenade type (+0x180) */
        if ( (unsigned __int16)grenade_type != 0xFFFF )
        {
            __int16 min_grenades = variant_definition->items.grenades_lower_bound;       /* +0x1D0 */
            __int16 max_grenades = variant_definition->items.grenades_upper_bound + 1;   /* +0x1D2, exclusive upper bound */
            unsigned int *seed = get_global_random_seed_address();
            __int16 desired = seed_random_range(seed, min_grenades, max_grenades);
            __int16 current_grenade_type = unit_get_current_grenade_type(actor->meta.unit_index);
            __int16 current_count = unit_get_grenade_count(actor->meta.unit_index, current_grenade_type);
            if ( current_count < desired )
                unit_add_grenade_type_to_inventory(actor->meta.unit_index, grenade_type, desired - current_count);
        }
    }
}
