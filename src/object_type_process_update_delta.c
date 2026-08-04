/* object_type_process_update_delta @0x837026F4 — apply a network update delta to an object: verifies the
 * object exists (any type), then finds the LAST (highest-index) part definition in its type's chain that has
 * a process_update_delta callback set and calls only that one (searches part_definitions[15] down to [0],
 * unlike the export_function_values/disconnect_from_structure_bsp fan-outs which call every part in forward
 * order). If the object can't be verified, the update is instead discarded via
 * message_delta_processor_discard_iteration_body.c (same tier) so the bitstream cursor still advances past it. */

#include <stdint.h>
#include "headers/object_type_definition.h"
#include "headers/message_delta_processor_header.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"

typedef struct network_game_client network_game_client;

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);

void object_type_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client)
{
    object_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_all);
    if ( !object )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    object_type_definition *type_definition = object_type_definitions[object->object.type];

    for ( __int16 i = 15; i >= 0; i-- )
    {
        object_type_definition *part = type_definition->part_definitions[i];
        if ( part && part->process_update_delta )
        {
            part->process_update_delta(object_index, header, client);
            return;
        }
    }
}
