/* damage_dealt_to_network @0x836B1678 — notify the network of damage an owner player dealt to another
 * player's biped, so the attacker's client can play a hit confirmation. Only fires when the damaged
 * object is a living biped (type 0, flags 0) controlled by a different player. On a dedicated server
 * (the damaged player's local_player_index == 0xFFFF, i.e. a non-local server-owned player) it encodes and sends a "damage dealt" message-delta to the
 * owner's machine; otherwise (local) it just plays the multiplayer hit sound.
 *
 * The damage_dealt_network_data is passed by value (r4/r5). The message-delta encoder, field-index
 * translator and network server are the Blam network-message subsystem (extern boundary). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/damage_dealt_network_data.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/networked_datum_role.h"
#include "headers/object_type.h"
#include "headers/message_delta_message_ids.h"

typedef struct network_game_server network_game_server;
struct field_properties_definition;

/* wire selector for network_game_server_send_message_to_machine (r5=1 at 0x836B174C). */
#define NETWORK_MESSAGE_TYPE_MESSAGE_DELTA 1
/* send priority (stack slot, li r11,9 at 0x836B1730). */
#define MESSAGE_DELTA_SEND_PRIORITY        9
#define MESSAGE_ENCODE_BUFFER_BITS         0x7FF8

#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
typedef struct _field_properties_definition _field_properties_definition;
extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern void play_multiplayer_hit_sound(void);

void damage_dealt_to_network(int owner_player_index, damage_dealt_network_data data)
{
    player_datum *player = datum_try_and_get(player_data, owner_player_index);
    if ( !player )
        return;

    unit_datum *object = object_try_and_get_and_verify_type(data.damaged_object_index, object_mask_unit);
    if ( !object )
        return;

    if ( object->object.type != object_type_biped                             /* not a biped */
      || object->object.datum_role != _networked_datum_master                       /* not locally simulated */
      || object->unit.player_index == owner_player_index )    /* controlled by the attacker */
        return;

    if ( (uint16_t)player->local_player_index == 0xFFFF )
    {
        damage_dealt_network_data payload = data;
        payload.damaged_object_index = field_translated_index_translate_index_no_default(
            &field_properties_object_index_definition, data.damaged_object_index);

        int size_in_bits = message_delta_processor_encode_stateless(
            _message_damage_dealt, 0, &payload, g_message_encode_buffer, MESSAGE_ENCODE_BUFFER_BITS);

        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_machine(
            server, player->network_player_data.machine_index, NETWORK_MESSAGE_TYPE_MESSAGE_DELTA,
            g_message_encode_buffer, size_in_bits, 0, 0, 0, MESSAGE_DELTA_SEND_PRIORITY);
    }
    else
    {
        play_multiplayer_hit_sound();
    }
}
