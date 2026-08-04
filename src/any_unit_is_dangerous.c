/* any_unit_is_dangerous @0x836CA700 — true if any unit (type mask 3, connected-objects flag set) is in
 * a dangerous animation state: mid grenade throw with the grenade not yet released, or being flung as
 * a dead body (airborne-dead / landing-dead) with translation applied (ignore-translation flag clear). */

#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/unit_datum.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/object_type.h"

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);

uint8_t any_unit_is_dangerous(void)
{
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_unit, 1u);
    unit_datum *unit = object_iterator_next(&iterator);
    if ( !unit )
        return 0;
    while ( 1 )
    {
        int animation_state = unit->unit.animation.state;
        if ( animation_state == _unit_state_throw_grenade && (unsigned __int8)unit->unit.grenade_throw_state != _grenade_throw_state_released )
            break;
        if ( (animation_state == _unit_state_dying || animation_state == _unit_state_dying_airborne) && (unit->unit.animation.flags & (1u << _unit_animation_ignore_translation_bit)) == 0 )
            break;
        unit = object_iterator_next(&iterator);
        if ( !unit )
            return 0;
    }
    return 1;
}
