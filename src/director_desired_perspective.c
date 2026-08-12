/* director_desired_perspective @0x836E47C8 — decides the camera perspective a unit's director should use
 * based on the seat it occupies. Writes a seat_state code (0 none, 1 entering, 2 riding, 3 exiting) and
 * returns the desired perspective (_director_perspective_first_person / _director_perspective_third_person).
 * A unit not riding anything (parent_object_index == -1) yields seat_state 0 and the first-person
 * perspective; the enter/exit states force third person regardless of the seat's own camera.
 *
 * Both seat bits come from the parent unit definition's seat block, indexed by the unit's seat index:
 * _unit_seat_third_person_on_enter_bit (6) gates the enter/exit transition states,
 * _unit_seat_has_third_person_camera_bit (4) selects the steady-state perspective. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation_state.h"
#include "headers/seat_state.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"


int16_t director_desired_perspective(int unit_index, int16_t *seat_state)
{
    *seat_state = _not_in_seat;
    int result = _director_perspective_first_person;

    if (unit_index != -1)
    {
        unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        int parent_index = unit->object.parent_object_index;
        if (parent_index != -1)
        {
            int third_person_on_enter = 0;
            object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum;
            /* parent must be a biped (type 0) or vehicle (type 1) */
            if (((1 << parent->object.type) & object_mask_unit) != 0)
            {
                unit_definition *parent_definition =
                    TAG_GET(unit_definition, parent->definition_index);
                const unit_seat *seat =
                    &((const unit_seat *)parent_definition->unit.seats.address)[unit->unit.parent_seat_index];
                third_person_on_enter = (seat->flags & (1u << _unit_seat_third_person_on_enter_bit)) != 0;
                result = (seat->flags & (1u << _unit_seat_has_third_person_camera_bit)) != 0
                       ? _director_perspective_third_person
                       : _director_perspective_first_person;
            }

            if (!third_person_on_enter)
            {
                *seat_state = _seat_idle;
            }
            else if (unit->unit.animation.state == _unit_state_entering_seat)
            {
                *seat_state = _entering_seat;
            }
            else
            {
                *seat_state = (unit->unit.animation.state == _unit_state_exiting_seat) ? _exiting_seat : _seat_idle;
            }
        }
    }

    int state = *seat_state;
    if (state == _entering_seat || state == _exiting_seat)
        return _director_perspective_third_person;
    return result;
}
