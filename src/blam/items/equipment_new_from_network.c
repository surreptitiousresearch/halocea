/* equipment_new_from_network @0x837B6058 — (client) apply an "equipment spawn" message-delta: decode the
 * equipment's definition, owner team/player/object (as translated network indices), placement transform, object
 * flags, and initial linear/angular velocity, then create a puppet equipment object at that transform and prime
 * its client-side prediction state. The decoded up vector is re-orthogonalized against the forward vector (up =
 * forward x (up x forward)) and both are normalized before placement. The player and owner-object indices are
 * mapped from network-translated to local indices; the fresh object's translated index is registered. After
 * creation the decoded flags are OR'd into the object flags (@+16) and the initial position and linear/angular
 * velocity are written into the object's gearbox prediction fields (position @+584, linear velocity @+596,
 * angular velocity @+608) and applied (gearbox_object_translate); the velocities are then mirrored into the
 * object's live velocity fields (@+104 / @+140). No-op for incremental-mode headers.
 *
 * DEVIATION: the decoder's by-ref destination is one contiguous buffer the decompiler split into many stack
 * locals — reproduced as a single struct (same pattern as projectile_new_from_network). The placement zero-init
 * the decompiler rendered as a `__PAIR64__(owner_player_index, 0)` store loop is a plain memset-to-0 (the same
 * construct the projectile sibling confirmed against disasm has r9=0). Object prediction fields use the DB equipment_datum members
 * per the net-delta corpus convention. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/networked_datum_role.h"
#include "headers/equipment_spawn_message.h"
#include "headers/object_header_datum.h"
#include "headers/equipment_datum.h"
#include <string.h>
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern float normalize3d(real_vector3d *v);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern uint8_t field_translated_index_client_register_local_and_translated_index(const _field_properties_definition *const field_properties_definition, const int local_index, const int translated_index);
extern void gearbox_object_translate(int object_index, const real_point3d *new_position);

void equipment_new_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _equipment_spawn_message message;
    if ( message_delta_processor_decode_stateless(&message, header) != 1 )
        return;

    /* re-orthogonalize up against forward: up = forward x (up x forward), then normalize both */
    real_vector3d forward = message.forward;
    real_vector3d up_in = message.up;
    float a = forward.n[0] * up_in.n[1] - up_in.n[0] * forward.n[1];
    float b = up_in.n[2] * forward.n[1] - forward.n[2] * up_in.n[1];
    float c = up_in.n[0] * forward.n[2] - up_in.n[2] * forward.n[0];
    real_vector3d up;
    up.n[0] = c * forward.n[2] - a * forward.n[1];
    up.n[1] = a * forward.n[0] - b * forward.n[2];
    up.n[2] = b * forward.n[1] - c * forward.n[0];
    normalize3d(&forward);
    normalize3d(&up);

    int owner_object_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
            message.owner_object_index);
    int owner_player_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
            message.owner_player_index);

    object_placement_data placement;
    memset(&placement, 0, sizeof(placement));
    placement.definition_index = message.definition_index;
    placement.owner_team_index = message.owner_team_index;
    placement.owner_player_index = owner_player_index;
    placement.owner_object_index = owner_object_index;
    placement.position = message.position;
    placement.forward = forward;
    placement.up = up;

    int object_index = object_new_with_datum_role_control(&placement, _networked_datum_puppet);
    if ( object_index == -1 )
        return;

    field_translated_index_client_register_local_and_translated_index(&field_properties_object_index_definition,
            object_index, message.server_object_index);

    equipment_datum *object = (equipment_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    object->object.flags |= message.object_flags;
    object->equipment.baseline.position.n[0] = message.position.n[0];
    object->equipment.baseline.position.n[1] = message.position.n[1];
    object->equipment.baseline.position.n[2] = message.position.n[2];
    object->equipment.baseline.translational_velocity = message.translational_velocity;
    object->equipment.baseline.angular_velocity = message.angular_velocity;
    object->equipment.baseline_index = message.current_baseline_index;
    object->equipment.baseline_valid = 1;
    object->equipment.message_index = 0;
    gearbox_object_translate(object_index, &object->equipment.baseline.position);
    object->object.translational_velocity = object->equipment.baseline.translational_velocity;
    object->object.angular_velocity = object->equipment.baseline.angular_velocity;
}
