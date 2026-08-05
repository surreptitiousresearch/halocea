/* projectile_new_from_network @0x8375A6F8 — (client) apply a "projectile spawn" message-delta: decode the
 * projectile's definition, owner team/player/object (as translated network indices), placement transform, and
 * initial linear/angular velocity, then create a puppet projectile object at that transform and prime its
 * client-side prediction state. The decoded up vector is re-orthogonalized against the forward vector (up =
 * forward x (up x forward)) and both are normalized before placement. The player and owner-object indices are
 * mapped from network-translated to local indices; the fresh object's translated index is registered. After
 * creation the initial position and velocity are written into the object's gearbox prediction fields
 * (position @+636, velocity @+648) and applied (gearbox_object_translate), and the velocity is mirrored into
 * the object's live velocity (@+104). No-op for incremental-mode headers.
 *
 * DEVIATION: the decoder's by-ref destination is one contiguous buffer the decompiler split into many stack
 * locals — reproduced as a single struct (same pattern as netgame_equipment_new_from_network). The placement
 * zero-init the decompiler rendered as `__PAIR64__(player,0)` stores is a plain memset-to-0 (disasm 0x8375A7D0:
 * `stdu r9,8(r11)` with r9=0). Object prediction fields are raw offsets per the net-delta corpus convention. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/networked_datum_role.h"
#include "headers/projectile_spawn_message.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
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

void projectile_new_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _projectile_spawn_message message;
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
    placement.angular_velocity = message.angular_velocity;
    placement.flags |= 2;

    int object_index = object_new_with_datum_role_control(&placement, _networked_datum_puppet);
    if ( object_index == -1 )
        return;

    field_translated_index_client_register_local_and_translated_index(&field_properties_object_index_definition,
            object_index, message.server_object_index);

    projectile_datum *object =
            (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    object->projectile.baseline.position.n[0] = message.position.n[0];
    object->projectile.baseline.position.n[1] = message.position.n[1];
    object->projectile.baseline.position.n[2] = message.position.n[2];
    object->projectile.baseline.translational_velocity.n[0] = message.translational_velocity.n[0];
    object->projectile.baseline.translational_velocity.n[1] = message.translational_velocity.n[1];
    object->projectile.baseline.translational_velocity.n[2] = message.translational_velocity.n[2];
    object->projectile.baseline_index = message.current_baseline_index;
    object->projectile.baseline_valid = 1;
    object->projectile.message_index = 0;
    gearbox_object_translate(object_index, &object->projectile.baseline.position);
    /* DEVIATION: the decompiler's word-punned dword copy (lwz/stw @0x8375A900-0x8375A914) is a plain
       real_vector3d copy — struct assignment is bit-exact */
    object->object.translational_velocity = object->projectile.baseline.translational_velocity;
}
