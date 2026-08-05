/* vehicle_process_update_delta @0x8375E288 — client-side decode counterpart of vehicle_build_update_delta.c:
 * apply an incoming network update for a vehicle object. Bails (discarding the iteration body) if the object
 * handle is stale/wrong-typed or object_type_is_update_valid rejects the update. Otherwise decodes the 64-byte
 * _vehicle_update_payload (position / translational_velocity / angular_velocity / forward / up, plus a
 * flags-bit5 byte), sets the "network blocked" object flag (0x8000000), and applies the custom-header-gated
 * writes (sequence number (vehicle.message_index) unconditional; when custom_header[6] is set, refresh the persisted baseline
 * (vehicle.baseline) and the baseline index). It then re-orthonormalizes the decoded forward/up frame and
 * normalizes both, stores the decoded state into the object's server-authoritative blend targets
 * (last_server_*) and its live fields, and places the vehicle (vehicle_set_position_and_correct_children).
 * A hard-snap re-application of position/velocity/orientation fires when the placed position drifts >10 units
 * from the request, the object is at rest (object flag bit 5, _object_at_rest_bit), or the up vectors disagree by more than ~45deg.
 * Finally it validates the update timestamp and stashes a copy of the payload in vehicle.last_network_data. The `client` parameter
 * is unused (kept for the DB-confirmed prototype). Sibling of biped_process_update_delta.c.
 *
 * Field offsets, the payload layout, and every store target were re-derived from disassembly
 * (0x8375E288-0x8375E5DC), not trusted from the decompiler's OVERLAPPED-register / _QWORD[18] display: the
 * object sub-struct sits at object_datum+4, so e.g. the network-blocked flag is object.flags and the position
 * compared in the snap test is object.position.
 *
 * DEVIATION: `message_transit_ticks` (decompiler v38, stack slot sp+0x50) is read but never written by this
 * function's own code — the message-delta decoder is presumed to populate it as an out-value the disassembly
 * does not attribute to the decode call. Reproduced as read as-is; it gates is_update_timestamp_valid and the
 * last_update_timestamp = custom_header.timestamp - transit computation. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/network_game_client.h"
#include "headers/object_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_update_payload.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include <string.h>
#include "headers/object_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t object_type_is_update_valid(int object_index, const message_delta_processor_mode mode, const int baseline_index_from_update, int message_index_from_update, const int latest_valid_baseline_index, int latest_valid_message_index, const int maximum_message_index);
extern uint8_t message_delta_processor_decode_incremental(void *const destination_data, const void *const baseline_data, const message_delta_processor_header *const header, const uint8_t allow_empty_body);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern float normalize3d(real_vector3d *v);
extern void vehicle_set_position_and_correct_children(int vehicle_index, real_point3d *new_position);
extern float __fsqrts(float x);

void vehicle_process_update_delta(int object_index, message_delta_processor_header *header,
    network_game_client *client)
{
    vehicle_datum *vehicle = object_try_and_get_and_verify_type(object_index, object_mask_vehicle);
    unsigned char *custom_header = (unsigned char *)header->custom_header;

    if ( !vehicle
        || !object_type_is_update_valid(object_index, header->decoding_information->mode, custom_header[4],
                custom_header[5], vehicle->vehicle.baseline_index, vehicle->vehicle.message_index, 255) )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _vehicle_update_payload payload;

    uint8_t decoded;
    if ( header->decoding_information->mode == _message_delta_mode_incremental )
    {
        memcpy(&payload, &vehicle->vehicle.baseline, sizeof(payload));
        decoded = message_delta_processor_decode_incremental(&payload, &vehicle->vehicle.baseline, header, 0);
    }
    else
    {
        decoded = message_delta_processor_decode_stateless(&payload, header);
    }

    if ( !decoded )
        return;

    vehicle->object.flags |= (1u << _object_has_been_updated_from_network_bit);
    vehicle->vehicle.message_index = custom_header[5];

    if ( custom_header[6] )
    {
        vehicle->vehicle.baseline_index = custom_header[4];
        memcpy(&vehicle->vehicle.baseline, &payload, sizeof(payload));
    }

    /* re-orthonormalize the decoded forward/up frame (originals captured first), then normalize both */
    float fx = payload.forward.n[0], fy = payload.forward.n[1], fz = payload.forward.n[2];
    float ux = payload.up.n[0], uy = payload.up.n[1], uz = payload.up.n[2];
    float cross_z_times_fy = (((fx * uy) - (fy * ux)) * fy);
    float new_up_z = ((fy * ((uz * fy) - (fz * uy)))
                           - (((fz * ux) - (uz * fx)) * fx));
    payload.up.n[1] = ((((fx * uy) - (fy * ux)) * fx)
                            - (fz * ((uz * fy) - (fz * uy))));
    payload.up.n[0] = ((((fz * ux) - (uz * fx)) * fz) - cross_z_times_fy);
    payload.up.n[2] = new_up_z;
    normalize3d(&payload.forward);
    normalize3d(&payload.up);

    vehicle->object.is_server_position_valid = 1;
    vehicle->object.is_server_translational_velocity_valid = 1;
    vehicle->object.is_server_orientation_valid = 1;
    vehicle->object.last_server_translational_velocity = payload.translational_velocity;
    vehicle->object.last_server_forward = payload.forward;
    vehicle->object.last_server_up = payload.up;
    vehicle->object.last_server_position = payload.position;
    if ( !payload.at_rest_bit )
        vehicle->object.flags &= ~(1u << _object_at_rest_bit);
    vehicle->object.translational_velocity = payload.translational_velocity;
    vehicle->object.angular_velocity = payload.angular_velocity;
    vehicle->object.forward = payload.forward;
    vehicle->object.up = payload.up;
    vehicle_set_position_and_correct_children(object_index, &payload.position);

    /* hard-snap when the placement drifted, the object is network-blocked, or the up axes disagree > ~45deg */
    float dx = payload.position.n[0] - vehicle->object.position.n[0];
    float dz = payload.position.n[2] - vehicle->object.position.n[2];
    float dy = payload.position.n[1] - vehicle->object.position.n[1];
    float up_dot = ((vehicle->object.up.n[0] * payload.up.n[0])
                         + ((vehicle->object.up.n[2] * payload.up.n[2])
                                 + (vehicle->object.up.n[1] * payload.up.n[1])));
    if ( __fsqrts(((dx * dx) + ((dz * dz) + (dy * dy)))) > 10.0f
        || (vehicle->object.flags & (1u << _object_at_rest_bit)) != 0
        || up_dot < 0.70700002f )
    {
        vehicle->object.last_server_position = payload.position;
        vehicle->object.last_server_translational_velocity = payload.translational_velocity;
        vehicle->object.forward = payload.forward;
        vehicle->object.up = payload.up;
        vehicle_set_position_and_correct_children(object_index, &payload.position);
    }

    int message_transit_ticks;   /* DEVIATION: a stack local (sp+0x50) read but never written here — see header note */
    if ( message_transit_ticks <= 10 )
    {
        vehicle->object.is_update_timestamp_valid = 0;
    }
    else
    {
        vehicle->object.is_update_timestamp_valid = 1;
        /* custom_header is an opaque wire header (void*); dword[2] is the update timestamp */
        int *custom_header_words = (int *)custom_header;
        vehicle->object.last_update_timestamp = custom_header_words[2] - message_transit_ticks;
    }

    vehicle->object.is_server_position_valid = 1;
    vehicle->object.is_server_translational_velocity_valid = 1;
    vehicle->unit.is_from_network_data_valid = 1;
    memcpy(&vehicle->vehicle.last_network_data, &payload, sizeof(payload));
}
