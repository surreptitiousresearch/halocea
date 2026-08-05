/* unit_can_play_animation_impulse @0x836C9630 — gate whether a unit may currently start an AI animation
 * impulse. False while its animation control block is busy. A free-standing unit (no parent) may play any
 * impulse except the vehicle celebrate/panic pair. A seated unit (parent of object type unit) may only play
 * the vehicle celebrate/panic impulses, and only if its seat allows vehicle communication animations
 * (unit_seat flags bit _unit_seat_allow_vehicle_communication_animations_bit, from the parent unit
 * definition's seats block). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/unit_animation_impulse.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

typedef struct unit_animation unit_animation;
extern uint8_t unit_animation_busy(unit_animation *animation);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

uint8_t unit_can_play_animation_impulse(int unit_index, int16_t animation_impulse)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    if ( unit_animation_busy(&unit->unit.animation) )
        return 0;

    int parent = unit->object.parent_object_index;
    if ( parent == -1 )
        return animation_impulse < _unit_animation_impulse_vehicle_celebrate
               || animation_impulse > _unit_animation_impulse_vehicle_panic;

    if ( (uint16_t)unit->unit.parent_seat_index == 0xFFFF )   /* cast: header field is signed */
        return 0;

    int *parent_object = object_try_and_get_and_verify_type(parent, object_mask_unit);
    if ( !parent_object
         || animation_impulse < _unit_animation_impulse_vehicle_celebrate
         || animation_impulse > _unit_animation_impulse_vehicle_panic )
        return 0;

    unit_definition *parent_definition = TAG_GET(unit_definition, *parent_object);
    unit_seat *seat = &((unit_seat *)parent_definition->unit.seats.address)[unit->unit.parent_seat_index];
    return (seat->flags >> _unit_seat_allow_vehicle_communication_animations_bit) & 1;
}
