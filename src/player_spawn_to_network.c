/* player_spawn_to_network 0x836A93B0 — encode a player-spawn message-delta for a unit and send it to the
 * network (to a single machine, or to all loaded machines when machine_index == -1). The message carries the
 * spawning player/unit/team, the unit's secondary (vehicle) object, its grenade counts, its weapon-inventory
 * object indices, and the player's two powerup-duration words. Object/player indices are translated to their
 * network-stable forms via field_translated_index_translate_index before transmission.
 *
 * DEVIATION 1: the decompiler models the contiguous on-stack message payload as two adjacent buffers
 * (spawn_header int[4] immediately followed by spawn_body __int16[48]); the encoder reads the whole payload
 * starting at spawn_header. The 32-bit translated weapon-object indices are written into spawn_body through an
 * int* cursor that overlaps the 16-bit body fields, exactly as in the original packed layout.
 * DEVIATION 2: the to-machine send passes a priority argument that the original reads from an uninitialized
 * stack slot (the decompiler's v16); reproduced as an uninitialized local so the call shape matches. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/field_properties_definition.h"
#include "headers/message_definition.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);

extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void player_spawn_to_network(int player_index, int unit_index, int team_index, int machine_index)
{
    unit_datum *unit_object_data;
    int primary_weapon_type;
    const int *weapon_slot_cursor;
    int *body_dword_cursor;
    int remaining_slots;
    player_datum *player;
    int encoded_size_in_bits;
    network_game_server *server;
    int priority_to_machine;
    int spawn_header[4];
    int16_t spawn_body[48];

    unit_object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    spawn_header[0] = field_translated_index_translate_index(&field_properties_player_index_definition, player_index);
    spawn_header[1] = field_translated_index_translate_index(&field_properties_object_index_definition, unit_index);
    spawn_header[2] = team_index;
    spawn_header[3] = field_translated_index_translate_index(&field_properties_object_index_definition,
                          unit_object_data->object.parent_object_index);

    spawn_body[0] = unit_object_data->unit.parent_seat_index;
    primary_weapon_type = unit_object_data->unit.current_weapon_index;
    if ( primary_weapon_type == -1 )
        /* set low 16 bits only, preserving the high word (0xFFFF here) — was LOWORD(...) lvalue */
        primary_weapon_type = (primary_weapon_type & ~0xFFFF) | (uint16_t)unit_object_data->unit.desired_weapon_index;
    /* int cursor anchored at desired_weapon_index (+756); pre-increment lands on weapon_object_indices[0..3] */
    weapon_slot_cursor = (const int *)&unit_object_data->unit.desired_weapon_index;
    spawn_body[10] = primary_weapon_type;

    body_dword_cursor = (int *)spawn_body;
    remaining_slots = 4;
    do
    {
        ++weapon_slot_cursor;
        --remaining_slots;
        *++body_dword_cursor = field_translated_index_translate_index(&field_properties_object_index_definition,
                                   *weapon_slot_cursor);
    }
    while ( remaining_slots );

    player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    spawn_body[11] = player->powerup_durations[0];
    spawn_body[12] = player->powerup_durations[1];

    encoded_size_in_bits = message_delta_processor_encode_stateless(_message_player_spawn, 0, spawn_header,
                               g_message_encode_buffer, 32760);
    if ( encoded_size_in_bits > 0 )
    {
        server = global_network_game_server_get();
        if ( machine_index == -1 )
            network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                g_message_encode_buffer, encoded_size_in_bits, 1u, 0, 0, 3);
        else
            network_game_server_send_message_to_machine(server, machine_index, network_message_type_message_delta,
                g_message_encode_buffer, encoded_size_in_bits, 1u, 0, 1u, priority_to_machine);
    }
}
