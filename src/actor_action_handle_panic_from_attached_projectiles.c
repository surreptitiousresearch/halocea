/* actor_action_handle_panic_from_attached_projectiles @0x837F1F58 — when an actor has a live "attached
 * projectile" object (+432 object index), resolve the responsible unit behind it and, if this is the first
 * time noticing it (or the actor had no prior panic-source unit, +780), remember that unit as the panic
 * source. Raises panic_type (+776) to at least _actor_panic_delayed_projectile_attached_to_us. Returns whether an attached projectile was
 * present at all.
 *
 * Fields resolved to named actor_datum members (attached-projectile object index +432, panic-source unit
 * index +780, panic timer +776). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_panic_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int ai_get_responsible_unit(int object_index, uint8_t responsible_for_weapon_fire);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);

uint8_t actor_action_handle_panic_from_attached_projectiles(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    int attached_projectile_index = actor->input.delayed_attached_projectile_index;
    if ( attached_projectile_index == -1 )
        return 0;

    object_datum *attached_projectile = object_try_and_get_and_verify_type(attached_projectile_index, object_mask_all);
    int active_by_unit_index = -1;
    if ( attached_projectile )
    {
        int responsible_unit = ai_get_responsible_unit(attached_projectile->object.owner_object_index, 1);
        if ( responsible_unit != -1 )
            active_by_unit_index = prop_get_active_by_unit_index(actor_index, responsible_unit);
    }

    /* floor is a panic_type priority, not a tick count */
    __int16 panic_type = actor->stimuli.panic_type;
    if ( !panic_type || actor->stimuli.panic_prop_index == -1 )
        actor->stimuli.panic_prop_index = active_by_unit_index;

    if ( panic_type <= _actor_panic_delayed_projectile_attached_to_us )
        panic_type = _actor_panic_delayed_projectile_attached_to_us;
    actor->stimuli.panic_type = panic_type;

    return 1;
}
