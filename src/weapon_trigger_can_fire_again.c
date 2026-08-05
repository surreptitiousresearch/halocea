/* weapon_trigger_can_fire_again @0x836D98F8 — whether a weapon trigger has had enough ticks since its
 * last shot to fire again. Computes the trigger's effective period (seconds) by lerping between the
 * trigger definition's initial/final rate of fire using either the weapon's charge fraction
 * (weapon.primary_trigger, when trigger definition flags bit 0x200 is set) or the per-trigger runtime
 * fraction (triggers[i].rate_of_fire), converts that to a 30Hz tick rate, and — if the weapon definition
 * has a nonzero age_rate_of_fire_penalty — scales it up using weapon.age. Compares that against the
 * trigger's idle_ticks counter. If the trigger definition additionally has flags bit 0x8 set and the
 * weapon's item.flags bit 0x2 is set, the result is further gated on triggers[i].flags bit 0x1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_definition_flags.h"
#include "headers/weapon_trigger_flags.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"


uint8_t weapon_trigger_can_fire_again(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *trigger_definition =
        &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index];

    float fraction;
    if ( trigger_definition->flags & (1u << _weapon_trigger_analog_rate_of_fire_bit) )
        fraction = weapon->weapon.primary_trigger;
    else
        fraction = trigger->rate_of_fire;

    float lower_bound = trigger_definition->initial_rate_of_fire;
    float upper_bound = trigger_definition->final_rate_of_fire;
    float period = (upper_bound - lower_bound) * fraction + lower_bound;

    float required_ticks = (period <= 0.0001f) ? 0.0f : (30.0f / period);

    float rate_modifier = definition->weapon.age_rate_of_fire_penalty;
    if ( rate_modifier > 0.0f )
        required_ticks = (weapon->weapon.age * rate_modifier + 1.0f) * required_ticks;

    uint8_t ticks_since_fire = trigger->idle_ticks;
    uint8_t result = ((float)ticks_since_fire + 1.0f) >= required_ticks;

    if ( (trigger_definition->flags & (1u << _weapon_trigger_latched_bit)) != 0 && (weapon->item.flags & (1u << _item_belongs_to_player_bit)) != 0 )
        return (trigger->flags & (1u << _weapon_trigger_released_since_last_shot_bit)) == 0 ? 0 : result;

    return result;
}
