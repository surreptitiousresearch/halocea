/* machine_new @0x837BBF30 — initializes a newly-created device-machine object's lighting flags. Always
 * flags the object for dynamic-lighting recompute; then, when the 'mach' tag definition is an elevator
 * (definition->machine.flags bit 2 = _machine_is_elevator, value 0x4), it additionally forces a static-
 * lighting recompute and a sideways static-lighting raycast (moving geometry must re-light). Non-elevator
 * machines clear those two static-lighting bits, keeping only the dynamic recompute flag. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/machine_flags.h"
#include "headers/object_flags.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/machine_definition.h"
#include "headers/blam_data_globals.h"


uint8_t machine_new(int machine_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, machine_index)->datum;

    machine_definition *definition = TAG_GET(machine_definition, object->definition_index);
    unsigned char is_elevator = (definition->machine.flags & (1u << _machine_is_elevator_bit)) != 0;

    unsigned int flags = object->object.flags;
    object->object.flags = flags | (1u << _object_dynamic_lighting_recompute_bit);

    flags = is_elevator
        ? (flags | ((1u << _object_dynamic_lighting_recompute_bit) | (1u << _object_static_lighting_recompute_bit)))
        : ((flags & ~((1u << _object_dynamic_lighting_recompute_bit) | (1u << _object_static_lighting_recompute_bit)))
                | (1u << _object_dynamic_lighting_recompute_bit));
    object->object.flags = flags;

    flags = object->object.flags;
    flags = is_elevator
        ? (flags | (1u << _object_static_lighting_raycast_sideways_bit))
        : (flags & ~(1u << _object_static_lighting_raycast_sideways_bit));
    object->object.flags = flags;

    return 1;
}
