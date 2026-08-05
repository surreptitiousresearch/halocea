/* netgame_equipment_new_from_network @0x83746FB0 — (client) apply a "netgame equipment spawn"
 * message-delta: decode the translated object index, tag definition index, and scenario netgame_equipment
 * list index, place a new puppet object at that equipment placement's position/facing (144-byte records;
 * position at +64, facing angle at +76), register its translated index, clear its garbage flag, and — if
 * the equipment record has _netgame_equipment_flag_float set — mark the object's _object_at_rest_bit.
 * Finally resync the object's replication baseline. No-op if the equipment list index has no entry.
 *
 * DEVIATION: the decompiler splits the decode destination into a `_DWORD[2]` array plus a trailing
 * `__int16` local; they are one contiguous by-ref decode buffer (translated_index, definition_index,
 * equipment_list_index), reproduced here as a single struct — same pattern as other *_from_network decode
 * call sites in this codebase. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/scenario.h"
#include "headers/object_placement_data.h"
#include "headers/scenario_netgame_equipment.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/real_vector3d.h"
#include "headers/networked_datum_role.h"
#include "headers/netgame_equipment_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);

extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern uint8_t field_translated_index_client_register_local_and_translated_index(const _field_properties_definition *const field_properties_definition, const int local_index, const int translated_index);
extern void object_set_garbage(int object_index, uint8_t garbage);
extern void object_type_update_baseline(int object_index);

typedef struct _netgame_equipment_spawn_message
{
    int     translated_object_index;
    int     definition_index;
    int16_t equipment_list_index;
} _netgame_equipment_spawn_message;

void netgame_equipment_new_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _netgame_equipment_spawn_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    scenario_netgame_equipment *equipment =
        &((scenario_netgame_equipment *)global_scenario->netgame_equipment.address)[message.equipment_list_index];
    if ( !equipment )
        return;

    object_placement_data placement;
    object_placement_data_new(&placement, message.definition_index, -1);
    placement.position.n[0] = equipment->position.n[0];
    placement.position.n[1] = equipment->position.n[1];
    placement.position.n[2] = equipment->position.n[2];
    vector3d_from_angle(&placement.forward, equipment->facing);

    int object_index = object_new_with_datum_role_control(&placement, _networked_datum_puppet);
    if ( object_index == -1 )
        return;

    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    field_translated_index_client_register_local_and_translated_index(&field_properties_object_index_definition,
        object_index, message.translated_object_index);
    object_set_garbage(object_index, 0);

    /* DB: object_datum.object (_object_datum) @ +4, .flags @ +12 — the raw form was dword index [4] */
    if ( (equipment->flags & (1u << _netgame_equipment_flag_float)) != 0 )
        object->object.flags |= (1u << _object_at_rest_bit);

    object_type_update_baseline(object_index);
}
