/* player_set_action_result_from_network @0x836AC428 — (client) apply a replicated player "action result"
 * message: decode {translated player index, is_weapon_swap flag, translated action object index,
 * action_result, action_seat_index, translated weapon-to-drop index}; resolve the object index and bail
 * (return 0) if it fails to resolve AND the wire value wasn't already NONE (a genuine resolution failure,
 * as opposed to "no object" being the intended value). Otherwise stores the resolved object
 * index/result/seat into the player datum and dispatches to player_handle_weapon_swap_client (when
 * is_weapon_swap) or player_handle_action. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *datum_try_and_get(const data_array *data, int index);
extern int player_handle_weapon_swap_client(int player_index, int weapon_to_drop_as_result_of_swap);
extern uint8_t player_handle_action(int player_index);

int player_set_action_result_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return 0;
    }

    struct
    {
        int translated_player_index;
        int is_weapon_swap;
        int translated_object_index;
        int16_t action_result;
        int16_t action_seat_index;
        int translated_weapon_to_drop_index;
    } payload;

    if ( !message_delta_processor_decode_stateless(&payload, header) )
        return 0;

    int local_player_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
                                  payload.translated_player_index);
    player_datum *player = datum_try_and_get(player_data, local_player_index);
    if ( !player )
        return 0;

    int local_object_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                                  payload.translated_object_index);
    int weapon_to_drop_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                                    payload.translated_weapon_to_drop_index);

    if ( local_object_index == -1 && payload.translated_object_index != -1 )
        return 0;

    player->action_object_index = local_object_index;
    player->action_result = payload.action_result;
    player->action_seat_index = payload.action_seat_index;

    if ( payload.is_weapon_swap )
        return player_handle_weapon_swap_client(local_player_index, weapon_to_drop_index);
    return player_handle_action(local_player_index);
}
