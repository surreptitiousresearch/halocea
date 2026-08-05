/* player_handle_powerup_from_network @0x836AD4D0 — apply a replicated powerup message. On the
 * decode-discard pass (decoding_information->mode set) the iteration body is skipped. Otherwise the
 * (translated_player_index, powerup_type, duration) payload is decoded, the translated player index
 * is mapped back to a local player index, and player_handle_powerup is invoked. Always returns 0. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern uint8_t player_handle_powerup(int player_index, int16_t powerup_type, int16_t duration_in_ticks);

int player_handle_powerup_from_network(message_delta_processor_header *header)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return 0;
    }

    struct
    {
        int     translated_player_index;
        int16_t powerup_type;
        int16_t duration_in_ticks;
    } payload;

    if ( message_delta_processor_decode_stateless(&payload, header) )
    {
        int local_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
                payload.translated_player_index);
        player_handle_powerup(local_index, payload.powerup_type, payload.duration_in_ticks);
    }
    return 0;
}
