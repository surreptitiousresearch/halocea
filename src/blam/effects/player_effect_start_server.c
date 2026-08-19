/* player_effect_start_server @0x837378D8 — on the server, encode a "player effect start" message-delta
 * (the screen flash / camera impulse / damage indicator triggered by a damage event) and send it
 * reliably to the affected player's machine. No-op if the player is invalid or marked local-only
 * (player byte +213). The message subsystem is the Blam network boundary (extern). */

#include <stdint.h>
#include "headers/iterated_message.h"   /* message_delta_processor_message_definition_type */
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/damage_data.h"
#include "headers/real_vector3d.h"

typedef struct network_game_server network_game_server;
struct field_properties_definition;

#include "headers/network_message_type.h"
#include "headers/message_delta_message_ids.h"

/* priority level passed to the server send; plain int priority (no DB enum found: searched
 * types_enum_values for network/message priority domains — value 3 is protocol-local). */
#define PLAYER_EFFECT_START_SEND_PRIORITY  3
#define MESSAGE_ENCODE_BUFFER_BITS         0x7FF8

#include "headers/field_properties_definition.h"

#include "headers/data_array.h"
typedef struct _field_properties_definition _field_properties_definition;
extern void *datum_try_and_get(const data_array *data, int index);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

#include "headers/player_effect_start_message.h"
#include "headers/blam_data_globals.h"

void player_effect_start_server(int player_index, const damage_data *damage,
                                const real_vector3d *direction, float scale, float total_damage)
{
    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( !player || player->quit_out_of_game )
        return;

    player_effect_start_message message;
    message.damage_definition_index = damage->definition_index;
    message.damage_owner_object_index = field_translated_index_translate_index(
        &field_properties_object_index_definition, damage->owner_object_index);
    message.damage_flags = damage->flags;
    message.direction.n[0] = direction->n[0];
    message.direction.n[1] = direction->n[1];
    message.direction.n[2] = direction->n[2];
    message.scale = scale;
    message.total_damage = total_damage;

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_player_effect_start, 0, &message, g_message_encode_buffer, MESSAGE_ENCODE_BUFFER_BITS);
    if ( size_in_bits > 0 )
    {
        int machine_index = player->network_player_data.machine_index;  /* char member -- same signed byte read */
        network_game_server *server = global_network_game_server_get();
        if ( machine_index != -1 )
            network_game_server_send_message_to_machine(
                server, machine_index, network_message_type_message_delta,
                g_message_encode_buffer, size_in_bits, 1, 0, 1, PLAYER_EFFECT_START_SEND_PRIORITY);
    }
}
