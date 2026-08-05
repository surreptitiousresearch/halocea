/* actor_action_consider_grenade @0x837F3190 — decide whether this actor should throw a grenade right
 * now. Short-circuits true if the actor already decided to throw one (control.grenade_trying_to_throw
 * flag). Otherwise, if
 * grenades are globally enabled and the actor's variant has both a primary and secondary grenade type
 * configured, re-rolls at most once per interval (tag def's grenade_throw_interval * 30 ticks): rolls
 * against a difficulty-scaled chance (grenade_throw_chance * team difficulty multiplier), and if the roll
 * succeeds and a valid grenade throw exists (actor_action_test_grenade), commits to throwing one.
 *
 * DEVIATION: the decompiler renders the interval re-check as a confusing __SPAIR64__ 64-bit-pair
 * construction; disasm shows it's just two independent int-to-float conversions compared directly
 * (current_time_as_float >= last_check_time_as_float + interval*30.0f), reproduced here as such. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/ai_globals.h"
#include "headers/game_difficulty_value.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern float game_difficulty_get_team_value(int16_t value_type, int16_t team_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern uint8_t actor_action_test_grenade(int actor_index);
extern int actor_action_try_to_throw_grenade(int actor_index, uint8_t known_trajectory);

int actor_action_consider_grenade(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    const actor_variant_definition *variant_tag = TAG_GET(const actor_variant_definition, actor->meta.variant_definition_index);

    if ( actor->control.grenade_trying_to_throw )
        return 1;

    if ( !ai_globals->grenades_enabled )
        return 0;

    if ( (uint16_t)variant_tag->grenade_combat.grenade_type == 0xFFFF
      || (uint16_t)variant_tag->grenade_combat.trajectory_type == 0xFFFF )
        return 0;

    int current_time = game_time_get();

    int last_check_time = actor->control.grenade_last_check_time;

    if ( last_check_time != -1
      && (float)current_time < (float)last_check_time
            + variant_tag->grenade_combat.throw_grenade_delay * 30.0f )
        return 0;

    float base_chance = variant_tag->grenade_combat.throw_grenade_chance;
    float difficulty_scale = game_difficulty_get_team_value(_game_difficulty_grenade_chance_scale, actor->meta.team_index);
    actor->control.grenade_last_check_time = current_time;
    float chance = difficulty_scale * base_chance;

    unsigned int *random_seed = get_global_random_seed_address();
    if ( real_seed_random(random_seed) < chance && actor_action_test_grenade(actor_index) )
    {
        actor->control.grenade_trying_to_throw = 1;
        actor_action_try_to_throw_grenade(actor_index, 1u);
        return 1;
    }

    return 0;
}
