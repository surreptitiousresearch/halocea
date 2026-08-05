/* weapon_start_reload_to_network @0x836D9A30 — (server only) broadcast a "weapon start reload" message-delta
 * to all loaded client machines so they mirror an authoritative reload. The message payload carries the
 * network-translated weapon object index plus the magazine index and its current loaded / reserve round
 * counts, encoded statelessly into the shared message-encode buffer and sent reliably at priority 3. */

#include <stdint.h>
#include "headers/iterated_message.h"   /* message_delta_processor_message_definition_type */
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"

#include "headers/field_properties_definition.h"
typedef struct _field_properties_definition _field_properties_definition;
extern int _message_weapon_start_reload;

typedef struct network_game_server network_game_server;

extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

#include "headers/weapon_start_reload_message.h"
#include "headers/blam_data_globals.h"
#include "headers/network_message_type.h"

void weapon_start_reload_to_network(int weapon_index, int16_t magazine_index)
{
    /* object_header element (+2 ints == +8 bytes) is the pointer to the object datum block */
    weapon_datum *weapon = ((weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum);

    weapon_start_reload_message message;
    message.weapon_index =
        field_translated_index_translate_index(&field_properties_object_index_definition, weapon_index);
    message.magazine_index = magazine_index;
    /* DEVIATION: the wire fields are populated crossed vs their names — loaded_rounds is filled from
     * magazines[].rounds_total (+694) and magazine_rounds from rounds_loaded (+696); reproduced faithfully. */
    message.starting_total_rounds   = weapon->weapon.magazines[magazine_index].rounds_total;
    message.starting_loaded_rounds = weapon->weapon.magazines[magazine_index].rounds_loaded;

    int size_in_bits = message_delta_processor_encode_stateless(_message_weapon_start_reload, 0, &message,
                                                                g_message_encode_buffer, 32760);

    struct network_game_server *server = global_network_game_server_get();
    /* trailing args per the DB prototype: reliable=1, immediate=0, including_local_client=0,
     * priority=3. The first three are plain booleans, not enum members; `priority` has no DB
     * enum (enum_oracle: nothing above noise). Every sibling caller in the corpus spells this
     * same `1u, 0, 0, 3` tail — see ctf_engine_replicate_game_mode_state_to_network.c. */
    network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits,
                                                            1u, 0u, 0u, 3);
}
