/* player_effect_start_from_network @0x83739018 — (client) apply a replicated "start player effect"
 * message: decode the damage_data payload (definition_index, translated owner object index, flags), find
 * the first local player, and hand it to player_effect_start_client with a zeroed direction/scale/total-
 * damage (not replicated over the wire — disasm confirms these share the same 32-byte zero-fill as the
 * payload buffer, and the decode only overwrites the first 12 bytes). */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/damage_data.h"
#include "headers/field_properties_definition.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void player_effect_start_client(int player_index, const damage_data *damage, const real_vector3d *direction, float scale, float total_damage);

void player_effect_start_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    int payload[3] = { 0, 0, 0 };
    if ( !message_delta_processor_decode_stateless(payload, header) )
        return;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return;

    while ( player->local_player_index == -1 )
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return;
    }

    real_vector3d direction = { 0.0f, 0.0f, 0.0f };
    float scale = 0.0f, total_damage = 0.0f;

    damage_data damage;
    damage.definition_index = payload[0];
    damage.owner_object_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                                     payload[1]);
    damage.flags = payload[2];

    player_effect_start_client(iterator.index, &damage, &direction, scale, total_damage);
}
