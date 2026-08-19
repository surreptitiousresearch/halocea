/* actor_action_handle_berserking_from_attached_projectiles @0x837F20C0 — chance to go berserk when a
 * projectile is stuck to the actor (attached-projectile prop dword +432 valid): requires visible-enemy
 * combat status (word +110 >= 5), the current target prop (dword +624; 312-byte props, distance float[71])
 * closer than the firing variant's berserk firing range upper bound, and a roll under the actor
 * definition's berserk chance (definition float +936). On success the berserk timer word (+784) is raised
 * to at least 4. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

uint8_t actor_action_handle_berserking_from_attached_projectiles(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_definition_tag = TAG_GET(actor_definition, actor->meta.definition_index);
    actor_variant_definition *firing_variant_definition = actor_combat_get_firing_variant_definition(actor_index);

    if ( actor->input.delayed_attached_projectile_index == -1 )
        return 0;
    if ( actor->state.combat_status < _actor_combat_status_clear_los )
        return 0;

    /* recovered: *(float *)((char *)variant + 0x64 + 264) -> ranged_combat.berserk_firing_range_upper_bound */
    if ( (DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index))->distance
      >= firing_variant_definition->ranged_combat.berserk_firing_range_upper_bound )
        return 0;

    if ( real_seed_random(get_global_random_seed_address()) >= actor_definition_tag->berserk.grenade_attached_chance )
        return 0;

    int16_t berserk_timer = actor->stimuli.berserk_type;
    if ( berserk_timer <= 4 )
        berserk_timer = 4;
    actor->stimuli.berserk_type = berserk_timer;
    return 1;
}
