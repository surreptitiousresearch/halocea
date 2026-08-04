/* actor_died @0x837205?? — handle an AI actor's death: randomly drop its current weapon and grenades, then
 * delete the actor and refresh its encounter. If the actor died in a controllable unit that still holds a
 * weapon (and has not already been told to persist control), it may keep "death-gripping" the trigger for a
 * randomized time (boosted when berserk/recently-charging) so the corpse fires a final burst. The dropped
 * weapon's battery/ammo and magazine rounds are randomized within the actor definition's ranges, and grenades
 * are cleared when grenades are globally disabled or a random roll falls under the definition's drop chance.
 *
 * Actor fields resolved to named actor_datum members; actor definition fields (resolved via the tag
 * instance) use the DB actor_variant_definition struct.
 *
 * DEVIATION: the decompiler packed the magazine value into a single int via LOWORD/HIWORD; on this big-endian
 * target the rounds_array it points at is {random_rounds, 0} (random in the high word at offset 0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/ai_globals.h"
#include "headers/actor_fire_target_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t unit_controllable(int unit_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void unit_persistent_control(int unit_index, int persistent_control_timer, unsigned int persistent_control_flags);
extern void weapon_set_current_amount(int weapon_index, float current_amount);
extern void weapon_set_total_rounds(int weapon_index, int16_t *rounds_array);
extern void actor_delete(int actor_index, uint8_t died);
extern void encounter_update_status(int encounter_index);

void actor_died(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int encounter_index = actor->meta.encounter_index;
    actor_variant_definition *definition = TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    int unit_index = actor->meta.unit_index;
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if (actor->state.mode == _actor_mode_combat && actor->state.combat_status >= _actor_combat_status_investigate)
    {
        if (unit_controllable(unit_index)
            && unit_inventory_get_weapon(unit_index, (unsigned __int16)unit_object->unit.current_weapon_index) != -1
            && unit_object->unit.weapon_drop_delay_ticks > 0)
        {
            float death_grip_chance = definition->ranged_combat.death_wildfire_chance;
            if (death_grip_chance < 0.1f)
                death_grip_chance = 0.1f;
            else if (death_grip_chance > 0.60000002f)
                death_grip_chance = 0.60000002f;

            if (actor->emotions.berserk || (actor->control.current_fire_target_type > actor_fire_target_none && actor->control.current_fire_target_distance < 3.0f))
            {
                float boosted = death_grip_chance * 4.0f;
                if (boosted > 0.60000002f)
                    boosted = 0.60000002f;
                if (death_grip_chance <= boosted)
                    death_grip_chance = boosted;
            }

            if (real_seed_random(get_global_random_seed_address()) < death_grip_chance)
            {
                float time_scale = definition->ranged_combat.death_wildfire_time;
                if (time_scale == 0.0f)
                {
                    time_scale = real_seed_random_range(get_global_random_seed_address(), 0.80000001f, 1.3f);
                }
                else if (time_scale < 0.80000001f)
                {
                    time_scale = 0.80000001f;
                }
                else if (time_scale > 1.3f)
                {
                    time_scale = 1.3f;
                }

                int control_ticks = (int)(time_scale * 30.0f);
                /* persistent_control mask is unit_control_flags (DB $DAB55C4E); 0x800 = fire primary weapon */
                unit_persistent_control(unit_index, control_ticks,
                                        1u << _unit_control_weapon_primary_trigger_bit);
                unit_object->unit.weapon_drop_delay_ticks = (char)control_ticks;
            }
        }
    }

    float grenade_roll = real_seed_random(get_global_random_seed_address());
    int weapon_index = unit_inventory_get_weapon(unit_index, (unsigned __int16)unit_object->unit.current_weapon_index);

    if (!ai_globals->grenades_enabled || grenade_roll < definition->items.dont_drop_grenades_chance)
        *(unsigned __int16 *)unit_object->unit.grenade_counts = 0;  /* zeroes both grenade counts */

    if (weapon_index != -1)
    {
        float ammo_minimum = definition->items.weapon_loaded_lower_bound;
        if (ammo_minimum > 0.0f || definition->items.weapon_loaded_upper_bound > 0.0f)
        {
            float ammo_maximum = definition->items.weapon_loaded_upper_bound;
            float amount = real_seed_random_range(get_global_random_seed_address(), ammo_minimum, ammo_maximum);
            weapon_set_current_amount(weapon_index, amount);
        }

        __int16 rounds_minimum = definition->items.weapon_ammo_lower_bound;
        if (rounds_minimum > 0 || definition->items.weapon_ammo_upper_bound > 0)
        {
            __int16 rounds_maximum = definition->items.weapon_ammo_upper_bound + 1;
            __int16 rounds_array[2];
            rounds_array[1] = 0;
            rounds_array[0] = seed_random_range(get_global_random_seed_address(), rounds_minimum, rounds_maximum);
            weapon_set_total_rounds(weapon_index, rounds_array);
        }
    }

    actor_delete(actor_index, 1);
    if (encounter_index != -1)
        encounter_update_status(encounter_index);
}
