/* projectile_process_update_delta @0x8375AAC0 — apply one incoming network update-delta message to a client's
 * projectile object. Verifies the object is a projectile (type mask 0x20) and that the update is valid for the
 * object's current sync sequence; if not, the message body is discarded. Otherwise it seeds the decode from the
 * object's stored baseline (6 dwords at +159), decodes the new state (incrementally from that baseline, or
 * statelessly), and on success commits it: sets sync flags, stores the sync id, position and derived ints, and
 * marks the object dirty. When the decode moved the projectile far enough (any stateless update, or an
 * incremental one past the position tolerance) and the object is client-authoritative for position (flag
 * 0x800), it teleports the object to the decoded position. Finally it records the update into the object's
 * history block (+166).
 *
 * DEVIATION: register-pun doubles are plain float math; the three hand-inlined 6-dword block copies (lwz/stw)
 * are written as projectile_datum_network_data struct assignments (bit-exact). The projectile object is
 * accessed through the DB-typed projectile_datum struct; the incremental decoder still sees the baseline as
 * raw dwords. `client` is part of the processor signature but unused by this handler. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/real_point3d.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_datum_network_data.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float);

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t object_type_is_update_valid(int object_index, const message_delta_processor_mode mode, const int baseline_index_from_update, int message_index_from_update, const int latest_valid_baseline_index, int latest_valid_message_index, const int maximum_message_index);
extern uint8_t message_delta_processor_decode_incremental(real_point3d *decoded_state,
        int *baseline_state, message_delta_processor_header *header, int flags);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern void gearbox_object_translate(int object_index, const real_point3d *new_position);

void projectile_process_update_delta(int object_index, message_delta_processor_header *header,
        network_game_client *client)
{
    projectile_datum *projectile = object_try_and_get_and_verify_type(object_index, object_mask_projectile);
    unsigned char *custom_header = (unsigned char *)header->custom_header;

    if ( !projectile
      || !object_type_is_update_valid(object_index, header->decoding_information->mode,
                custom_header[4], custom_header[5],
                projectile->projectile.baseline_index, projectile->projectile.message_index, 255) )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    /* baseline / last_network_data are 6-dword (position + translational_velocity) network blocks;
       the incremental decoder consumes the stored baseline as raw dwords */
    int *baseline = (int *)&projectile->projectile.baseline;

    projectile_datum_network_data decoded_state = projectile->projectile.baseline;  /* seed decode from the stored baseline */
    real_point3d *decoded_position = &decoded_state.position;

    uint8_t decoded;
    if ( header->decoding_information->mode == _message_delta_mode_incremental )
        decoded = message_delta_processor_decode_incremental(decoded_position, baseline, header, 0);
    else
        decoded = message_delta_processor_decode_stateless(decoded_position, header);
    if ( !decoded )
        return;

    projectile->object.flags |= (1u << _object_has_been_updated_from_network_bit);
    projectile->projectile.message_index = custom_header[5];
    if ( custom_header[6] )
    {
        projectile->projectile.baseline_index = custom_header[4];
        projectile->projectile.baseline = decoded_state;
    }

    projectile->object.translational_velocity = decoded_state.translational_velocity;
    projectile->object.last_server_position.n[0] = decoded_position->n[0];
    projectile->object.last_server_position.n[1] = decoded_position->n[1];
    projectile->object.last_server_position.n[2] = decoded_position->n[2];
    projectile->object.is_server_position_valid = 1;
    projectile->object.is_server_translational_velocity_valid = 1;
    projectile->object.last_server_translational_velocity = decoded_state.translational_velocity;

    if ( (header->decoding_information->mode != _message_delta_mode_incremental
       || __fsqrts((decoded_position->n[1] - projectile->object.position.n[1]) * (decoded_position->n[1] - projectile->object.position.n[1])
                 + ((decoded_position->n[0] - projectile->object.position.n[0]) * (decoded_position->n[0] - projectile->object.position.n[0])
                  + (decoded_position->n[2] - projectile->object.position.n[2]) * (decoded_position->n[2] - projectile->object.position.n[2])))
              > sPROJECTILE_CLIENT_TO_SERVER_POSITION_TOLERANCE_0)
      && (projectile->object.flags & (1u << _object_connected_to_map_bit)) != 0 )
    {
        gearbox_object_translate(object_index, decoded_position);
    }

    projectile->projectile.last_network_data_valid = 1;
    projectile->projectile.last_network_data = decoded_state;  /* record into the history block */
}
