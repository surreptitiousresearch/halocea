/* biped_process_update_delta @0x837ADE18 — the client-side decode counterpart of biped_build_update_delta.c:
 * apply an incoming network update for a biped object. Bails (discarding the iteration body) if the object
 * handle is stale/wrong-typed or object_type_is_update_valid rejects the update. Otherwise decodes the same
 * 16-byte payload biped_build_update_delta.c encodes from +1324 (biped_datum_network_data: grenade_counts / body_vitality / shield_vitality /
 * raw int), sets the "network blocked" object flag (0x8000000), and applies the two custom-header-gated
 * writes back to the live object: the sequence number (+1320, unconditional) and, when custom_header[6] is
 * set, a refresh of the persisted baseline at +1324..+1336 plus +1319. Independently of that gate, the
 * decoded payload always updates +798 (word) and +224 (float), and — when custom_header[7] == 1 — +228
 * (shield_vitality, tripled). A second, always-tripled copy of the payload is stashed at +1344..+1356. The
 * `client` parameter is unused, matching the decompiler (kept for the DB-confirmed prototype).
 *
 * DB field names: object.body_vitality (+224), object.shield_vitality (+228), object.shield_stun_ticks
 * (+260), unit.grenade_counts (+798), biped.baseline_index/message_index (+1319/+1320); the 16-byte wire
 * payload maps onto biped.baseline / biped.last_network_data. */

#include <stdint.h>
#include "headers/biped_datum.h"
#include "headers/message_delta_processor_header.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/network_game_client.h"
#include "headers/biped_update_body.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t object_type_is_update_valid(int object_index, const message_delta_processor_mode mode, const int baseline_index_from_update, int message_index_from_update, const int latest_valid_baseline_index, int latest_valid_message_index, const int maximum_message_index);
extern unsigned __int8 message_delta_processor_decode_incremental(void *const destination_data,
    const void *const baseline_data, const message_delta_processor_header *const header,
    const unsigned __int8 allow_empty_body);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);

void biped_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client)
{
    biped_datum *biped = object_try_and_get_and_verify_type(object_index, object_mask_biped);
    unsigned char *custom_header = header->custom_header;

    if ( !biped
        || !object_type_is_update_valid(object_index, header->decoding_information->mode, custom_header[4],
                custom_header[5], biped->biped.baseline_index, biped->biped.message_index, 255) )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _biped_update_body body = *&biped->biped.baseline;

    unsigned __int8 decoded;
    if ( header->decoding_information->mode == _message_delta_mode_incremental )
        decoded = message_delta_processor_decode_incremental(&body, &biped->biped.baseline, header, 0);
    else
        decoded = message_delta_processor_decode_stateless(&body, header);

    if ( !decoded )
        return;

    biped->object.flags |= (1u << _object_has_been_updated_from_network_bit);
    biped->biped.message_index = custom_header[5];

    if ( custom_header[6] )
    {
        biped->biped.baseline_index = custom_header[4];
        biped->biped.baseline = body; /* _biped_update_body == biped_datum_network_data */
    }

    *(__int16 *)biped->unit.grenade_counts = *(const __int16 *)body.grenade_counts; /* was payload_a>>16: BE bytes 0-1 = grenade_counts */
    biped->object.body_vitality = body.body_vitality;

    float tripled_crouch = body.shield_vitality * 3.0f;
    if ( custom_header[7] == 1 )
        biped->object.shield_vitality = tripled_crouch;

    biped->object.shield_stun_ticks = (body.shield_stun_ticks_greater_than_zero == 1); /* was payload_b>>24: BE byte 12 */
    biped->unit.is_from_network_data_valid = 1;
    biped->biped.last_network_data_valid = 1;

    *(int *)&biped->biped.last_network_data = *(const int *)&body; /* raw 4-byte copy: grenade_counts + pad */
    biped->biped.last_network_data.body_vitality = body.body_vitality;
    biped->biped.last_network_data.shield_vitality = tripled_crouch;
    *(int *)&biped->biped.last_network_data.shield_stun_ticks_greater_than_zero = *(const int *)&body.shield_stun_ticks_greater_than_zero; /* raw 4-byte copy: stun byte + trailing pad */
}
