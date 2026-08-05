/* weapon_trigger_begin_firing @0x836DCE90 — begin the firing action for one trigger of a weapon. Bails out
 * if the trigger has an assigned magazine that is empty, if the weapon's "cannot fire" flag is set, or if the
 * weapon is underwater. Otherwise: when not forced and the trigger has no charging time but a positive
 * "spin-up"/ramp time, it enters ramp state (state byte 1, timer = ramp * 30). When not forced and the
 * trigger has a positive charging time (and the weapon isn't a heat weapon at/over full heat), it begins
 * charging — for single-trigger weapons it either arms the trigger (setting the 0x20 flag and firing) or
 * disarms it depending on the trigger's charge fraction, and for multi-trigger weapons it spawns the charge
 * effect — then sets charge state (state byte 2, timer = charge * 30) and drives the HCEX "charging_start"
 * state. In every remaining case (forced, or the ramp/charge branches fall through) it fires immediately.
 * The "spin-up"/ramp time is the DB member overloading_time. The underwater/can-fire guard is inverted
 * to an early return. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_trigger_state.h"
#include "headers/weapon_trigger_flags.h"
#include "headers/weapon_datum_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern void weapon_trigger_fire(int weapon_index, int16_t trigger_index);
extern int weapon_effect_new(int weapon_index, int effect_index, float effect_scale, float effect_error);
extern void hcex_obj_set_state(int object_index, const char *state);

void weapon_trigger_begin_firing(int weapon_index, int16_t trigger_index, uint8_t force_firing)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *trigger_definition =
        &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index];

    /* a trigger with an assigned magazine can only begin firing while that magazine is idle */
    uint8_t magazine_idle = 1;
    int16_t magazine_index = trigger_definition->magazine_index;
    if ( (uint16_t)magazine_index != 0xFFFF )
        magazine_idle = weapon->weapon.magazines[magazine_index].state == _weapon_magazine_idle;

    bool can_fire = (weapon->weapon.flags & (1u << _weapon_overheated_bit)) == 0 && magazine_idle;

    if ( scenario_location_underwater(&weapon->object.location, &weapon->object.position, nullptr)
      || !can_fire )
        return;

    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    if ( !force_firing )
    {
        if ( trigger_definition->charging_time <= 0.0f )
        {
            if ( trigger_definition->overloading_time > 0.0f )
            {
                trigger->state = _weapon_trigger_firing;
                trigger->state_timer = (int16_t)(trigger_definition->overloading_time * 30.0f);
                return;
            }
        }
        else if ( (definition->weapon.flags & (1u << _weapon_cannot_fire_at_maximum_age_bit)) == 0 || weapon->weapon.age < 1.0f )
        {
            if ( definition->weapon.triggers.count <= 1 )
            {
                int trigger_flags = trigger->flags;
                if ( trigger->rate_of_fire <= 0.0f )   /* held-charge fraction */
                {
                    trigger->flags = trigger_flags & ~(1u << _weapon_trigger_fired_before_charging_bit);
                }
                else
                {
                    trigger->flags = trigger_flags | (1u << _weapon_trigger_fired_before_charging_bit);
                    weapon_trigger_fire(weapon_index, trigger_index);
                }
            }
            else
            {
                trigger->charging_effect_index = weapon_effect_new(weapon_index,
                        trigger_definition->charging_effect.index, 0.0f, 0.0f);
            }
            trigger->state = _weapon_trigger_charging;
            trigger->state_timer = (int16_t)(trigger_definition->charging_time * 30.0f);
            hcex_obj_set_state(weapon_index, "charging_start");
            return;
        }
    }

    weapon_trigger_fire(weapon_index, trigger_index);
}
