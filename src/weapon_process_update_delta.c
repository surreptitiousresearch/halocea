/* weapon_process_update_delta @0x836DAB10 — apply one network update-delta message to a client's weapon object
 * (object type mask 4). Same shape as the projectile/equipment handlers: verify the object and that the update
 * is valid for its sync sequence (else discard the body); seed the decode from the stored baseline (11 dwords at
 * +185), decode incrementally or statelessly, and on success commit position + secondary vector + two packed
 * sync words, mark the object dirty, teleport if it moved past tolerance (or a resync/authoritative-position
 * flag is set), and record the update into the history block (+197).
 *
 * DEVIATION: register-pun doubles are plain float math; the two packed sync shorts come from reinterpreting the
 * decoded dword at staging[3] as HIWORD/LOWORD. The weapon object is kept as raw dword/byte/float offsets;
 * `client` is unused by this handler. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/real_point3d.h"
#include "headers/weapon_datum.h"
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
extern void *memcpy(void *dst, const void *src, unsigned int size);

void weapon_process_update_delta(int object_index, message_delta_processor_header *header,
        network_game_client *client)
{
    weapon_datum *weapon = object_try_and_get_and_verify_type(object_index, object_mask_weapon);
    unsigned char *custom_header = (unsigned char *)header->custom_header;

    if ( !weapon
      || !object_type_is_update_valid(object_index, header->decoding_information->mode,
                custom_header[4], custom_header[5],
                weapon->weapon.baseline_index, weapon->weapon.message_index, 255) )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    real_point3d staging[8];
    memcpy(staging, &weapon->weapon.baseline, 0x2Cu);

    uint8_t decoded;
    if ( header->decoding_information->mode == _message_delta_mode_incremental )
        decoded = message_delta_processor_decode_incremental(staging,
                      (int *)&weapon->weapon.baseline, header, 0);
    else
        decoded = message_delta_processor_decode_stateless(staging, header);
    if ( !decoded )
        return;

    weapon->object.flags |= (1u << _object_has_been_updated_from_network_bit);
    weapon->weapon.message_index = custom_header[5];
    if ( custom_header[6] )
    {
        weapon->weapon.baseline_index = custom_header[4];
        memcpy(&weapon->weapon.baseline, staging, 0x2Cu);
    }

    weapon->object.translational_velocity.n[0] = staging[1].n[0];
    weapon->object.translational_velocity.n[1] = staging[1].n[1];
    weapon->object.translational_velocity.n[2] = staging[1].n[2];
    weapon->object.last_server_position.n[0] = staging[0].n[0];
    weapon->object.last_server_position.n[1] = staging[0].n[1];
    weapon->object.last_server_position.n[2] = staging[0].n[2];
    weapon->object.is_server_position_valid = 1;
    weapon->object.is_server_translational_velocity_valid = 1;
    weapon->object.last_server_translational_velocity.n[0] = staging[1].n[0];
    weapon->object.last_server_translational_velocity.n[1] = staging[1].n[1];
    weapon->object.last_server_translational_velocity.n[2] = staging[1].n[2];

    unsigned int packed = *(unsigned int *)&staging[3].n[0];  /* network encoding: two packed uint16 rounds totals in the float slot's bit image */
    weapon->weapon.magazines[0].rounds_total = packed >> 16;
    weapon->weapon.magazines[1].rounds_total = (uint16_t)packed;
    if ( header->decoding_information->mode == _message_delta_mode_stateless )
        weapon->weapon.age = staging[3].n[1];

    int flags = weapon->object.flags;
    if ( ((flags & (1u << _object_connected_to_map_bit)) != 0
       && __fsqrts((staging[0].n[1] - weapon->object.position.n[1]) * (staging[0].n[1] - weapon->object.position.n[1])
                 + ((staging[0].n[0] - weapon->object.position.n[0]) * (staging[0].n[0] - weapon->object.position.n[0])
                  + (staging[0].n[2] - weapon->object.position.n[2]) * (staging[0].n[2] - weapon->object.position.n[2])))
              > sPROJECTILE_CLIENT_TO_SERVER_POSITION_TOLERANCE)
      || (flags & (1u << _object_at_rest_bit)) != 0
      || custom_header[6] )
    {
        gearbox_object_translate(object_index, staging);
    }

    weapon->weapon.last_network_data_valid = 1;
    memcpy(&weapon->weapon.last_network_data, staging, 0x2Cu);
}
