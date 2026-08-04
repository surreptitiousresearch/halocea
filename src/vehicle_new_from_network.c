/* vehicle_new_from_network @0x8375DD38 — (client) apply a "vehicle spawn" message-delta: decode the vehicle's
 * definition, owner team/player/object (as translated network indices), the 4 translated weapon-object indices,
 * placement transform, and initial linear/angular velocity, then create a puppet vehicle at that transform and
 * prime its client-side prediction state. The decoded up vector is re-orthogonalized against forward
 * (up = forward x (up x forward)) and both are normalized before placement. After creation the vehicle's
 * prediction block (@+1317) is seeded from the message (position @+1324, velocities @+1336/+1348, transform
 * @+1360/+1372), applied (gearbox_object_translate), then mirrored into the live object motion fields
 * (velocity @+104, forward @+116, up @+128, angular velocity @+140); finally each of the 4 message weapon
 * indices is mapped to a local object and added to the vehicle's inventory (a failed mapping writes -1 into
 * the corresponding weapon-slot dword @+760). No-op for incremental-mode headers.
 *
 * 5th member of the *_new_from_network puppet-spawn family (projectile/equipment/weapon/biped); same
 * decode/reorthogonalize/register/OBJECT_DATA idioms. DEVIATION: the placement zero-init the decompiler rendered
 * as a __PAIR64__(owner_player_index, 0) store loop is a plain memset-to-0 (disasm 0x8375DE08: li r9,0;
 * stdu r9,8(r11)); the decode destination is one contiguous buffer reproduced as a single struct. Prediction/live
 * fields are raw object offsets per the net-delta convention. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/networked_datum_role.h"
#include "headers/vehicle_spawn_message.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/unit_add_weapon_mode.h"
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
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);

void vehicle_new_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _vehicle_spawn_message message;
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

    int vehicle_index = object_new_with_datum_role_control(&placement, _networked_datum_puppet);
    if ( vehicle_index == -1 )
        return;

    field_translated_index_client_register_local_and_translated_index(&field_properties_object_index_definition,
            vehicle_index, message.server_object_index);

    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;

    /* seed the client-prediction (baseline) block */
    vehicle->vehicle.baseline.position = message.position;
    vehicle->vehicle.baseline.translational_velocity = message.translational_velocity;
    vehicle->vehicle.baseline.angular_velocity = message.angular_velocity;
    vehicle->vehicle.baseline.forward = forward;
    vehicle->vehicle.baseline.up = up;
    vehicle->vehicle.baseline_index = message.current_baseline_index;
    vehicle->vehicle.baseline_valid = 1;
    vehicle->vehicle.message_index = 0;
    gearbox_object_translate(vehicle_index, &vehicle->vehicle.baseline.position);

    /* mirror the baseline block into the live object motion fields */
    vehicle->object.translational_velocity = vehicle->vehicle.baseline.translational_velocity;
    vehicle->object.angular_velocity = vehicle->vehicle.baseline.angular_velocity;
    vehicle->object.forward = vehicle->vehicle.baseline.forward;
    vehicle->object.up = vehicle->vehicle.baseline.up;
    vehicle->unit.is_from_network_data_valid = 1;

    int *weapon_slot = &vehicle->unit.weapon_object_indices[0];
    for ( int slot = 0; slot < 4; ++slot, ++weapon_slot )
    {
        int weapon_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                message.weapon_object_indices[slot]);
        if ( weapon_index == -1 )
            *weapon_slot = -1;
        else
            unit_add_weapon_to_inventory(vehicle_index, weapon_index, _unit_add_weapon_normally);
    }
}
