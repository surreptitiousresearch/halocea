/* equipment_process_update_delta @0x837B6458 — apply one network update-delta message to a client's equipment
 * object (object type mask 8). Same shape as the projectile/weapon handlers: verify the object and update
 * validity (else discard), seed the decode from the stored baseline (9 dwords at +146), decode incrementally or
 * statelessly, and on success commit position plus two secondary vectors, mark the object dirty, teleport if it
 * moved past tolerance or carries the _object_at_rest_bit (bit 5), and record the update into the history block (+155).
 *
 * DEVIATION: register-pun doubles are plain float math; the three hand-inlined 9-dword block copies are written
 * as loops. Committed vectors go through the typed `eq` view; the baseline/history dword blocks stay as raw
 * `equipment[...]` indices. `client` is unused by this handler. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/real_point3d.h"
#include "headers/equipment_datum.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float);

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t object_type_is_update_valid(int object_index, const message_delta_processor_mode mode, const int baseline_index_from_update, int message_index_from_update, const int latest_valid_baseline_index, int latest_valid_message_index, const int maximum_message_index);
extern unsigned __int8 message_delta_processor_decode_incremental(void *destination_data,
        const void *baseline_data, const message_delta_processor_header *header, unsigned __int8 allow_empty_body);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern void gearbox_object_translate(int object_index, const real_point3d *new_position);

void equipment_process_update_delta(int object_index, message_delta_processor_header *header,
        network_game_client *client)
{
    equipment_datum *eq = object_try_and_get_and_verify_type(object_index, object_mask_equipment);
    unsigned char *custom_header = (unsigned char *)header->custom_header;

    if ( !eq
      || !object_type_is_update_valid(object_index, header->decoding_information->mode,
                custom_header[4], custom_header[5],
                eq->equipment.baseline_index, eq->equipment.message_index, 255) )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    int decoded_state[9];
    real_point3d *decoded_position = (real_point3d *)decoded_state;
    for ( int i = 0; i < 9; i++ )
        decoded_state[i] = ((const int *)&eq->equipment.baseline)[i];

    unsigned __int8 decoded;
    if ( header->decoding_information->mode == _message_delta_mode_incremental )
        decoded = message_delta_processor_decode_incremental(decoded_position, (real_point3d *)&eq->equipment.baseline, header, 0);
    else
        decoded = message_delta_processor_decode_stateless(decoded_position, header);
    if ( !decoded )
        return;

    eq->object.flags |= (1u << _object_has_been_updated_from_network_bit);
    eq->equipment.message_index = custom_header[5];
    if ( custom_header[6] )
    {
        eq->equipment.baseline_index = custom_header[4];
        for ( int i = 0; i < 9; i++ )
            ((int *)&eq->equipment.baseline)[i] = decoded_state[i];
    }

    /* decoded_state holds serialized float bit-patterns; copy bit-exact into the float members */
    *(int *)&eq->object.translational_velocity.n[0] = decoded_state[3];
    *(int *)&eq->object.translational_velocity.n[1] = decoded_state[4];
    *(int *)&eq->object.translational_velocity.n[2] = decoded_state[5];
    *(int *)&eq->object.angular_velocity.n[0] = decoded_state[6];
    *(int *)&eq->object.angular_velocity.n[1] = decoded_state[7];
    *(int *)&eq->object.angular_velocity.n[2] = decoded_state[8];
    *(int *)&eq->object.last_server_position.n[0] = *(int *)&decoded_position->n[0];
    eq->object.last_server_position.n[1] = decoded_position->n[1];
    eq->object.last_server_position.n[2] = decoded_position->n[2];
    eq->object.is_server_position_valid = 1;
    eq->object.is_server_translational_velocity_valid = 1;
    *(int *)&eq->object.last_server_translational_velocity.n[0] = decoded_state[3];
    *(int *)&eq->object.last_server_translational_velocity.n[1] = decoded_state[4];
    *(int *)&eq->object.last_server_translational_velocity.n[2] = decoded_state[5];

    if ( __fsqrts((decoded_position->n[1] - eq->object.position.n[1]) * (decoded_position->n[1] - eq->object.position.n[1])
                + ((decoded_position->n[0] - eq->object.position.n[0]) * (decoded_position->n[0] - eq->object.position.n[0])
                 + (decoded_position->n[2] - eq->object.position.n[2]) * (decoded_position->n[2] - eq->object.position.n[2])))
              > sPROJECTILE_CLIENT_TO_SERVER_POSITION_TOLERANCE_1
      || (eq->object.flags & (1u << _object_at_rest_bit)) != 0 )
    {
        gearbox_object_translate(object_index, decoded_position);
    }

    eq->equipment.last_network_data_valid = 1;
    /* faithful: the binary stores the 9-int state starting at last_network_data_valid (+0x26C), so it
     * spans that flag byte + padding + last_network_data — kept byte-exact via its address. */
    for ( int i = 0; i < 9; i++ )
        ((int *)&eq->equipment.last_network_data_valid)[i] = decoded_state[i];
}
