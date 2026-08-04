/* item_accelerate_from_network @0x836B19F8 — apply a stateless _message_item_accelerate message from the
 * network: decode an item_accelerate_network_data payload, resolve the translated object index back to a
 * local one, and if valid, apply the impulse (direction scaled by magnitude) to the item. Discards the
 * body during the iteration/discard decode pass. */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/message_delta_processor_header.h"
#include "headers/item_accelerate_network_data.h"
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void item_accelerate(int item_index, const real_vector3d *acceleration, uint8_t detonates_explosives);

void item_accelerate_from_network(message_delta_processor_header *header)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    item_accelerate_network_data data;
    if ( !message_delta_processor_decode_stateless(&data, header) )
        return;

    int local_index = field_translated_index_get_local_index(
        &field_properties_object_index_definition, data.object_index);
    if ( local_index == -1 )
        return;

    real_vector3d acceleration;
    acceleration.n[0] = data.direction.n[0] * data.magnitude;
    acceleration.n[1] = data.direction.n[1] * data.magnitude;
    acceleration.n[2] = data.direction.n[2] * data.magnitude;
    item_accelerate(local_index, &acceleration, 0);
}
