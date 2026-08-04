/* director_desired_perspective @0x836E47D8 — decides the camera perspective a unit's director should use
 * based on the seat it occupies. Writes a seat_state code (0 none, 1 entering/exiting cinematic, 2 normal
 * ride, 3 first-person ride) and returns the desired perspective (_director_perspective_first_person /
 * _director_perspective_third_person). A unit not riding anything (no parent at +0x11C) yields seat_state 0
 * and the first-person perspective.
 *
 * The seat's flags (bit 6 = allows rider camera control, bit 4 = first person) come from the parent unit's
 * definition seat block indexed by the unit's seat index, 284-byte unit_seat records. */

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
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"


int16_t director_desired_perspective(int unit_index, int16_t *seat_state)
{
    *seat_state = 0;
    int result = _director_perspective_first_person;

    if (unit_index != -1)
    {
        unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        int parent_index = unit->object.parent_object_index;
        if (parent_index != -1)
        {
            int rider_camera = 0;
            object_datum *parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum;
            /* parent must be a biped (type 0) or vehicle (type 1) */
            if (((1 << parent->object.type) & object_mask_unit) != 0)
            {
                unit_definition *parent_definition =
                    TAG_GET(unit_definition, parent->definition_index);
                const unit_seat *seat =
                    &((const unit_seat *)parent_definition->unit.seats.address)[unit->unit.parent_seat_index];
                rider_camera = (seat->flags >> _unit_seat_third_person_on_enter_bit) & 1;
                result = (seat->flags & (1u << _unit_seat_has_third_person_camera_bit)) != 0;
            }

            if (!rider_camera)
            {
                *seat_state = 2;
            }
            else if (unit->unit.animation.state == _unit_state_entering_seat)
            {
                *seat_state = 1;
            }
            else
            {
                *seat_state = (unit->unit.animation.state == _unit_state_exiting_seat) ? 3 : 2;
            }
        }
    }

    int state = *seat_state;
    if (state == 1 || state == 3)
        return _director_perspective_third_person;
    return result;
}
