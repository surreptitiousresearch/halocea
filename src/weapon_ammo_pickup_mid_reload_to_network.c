/* weapon_ammo_pickup_mid_reload_to_network 0x836D87A8 — encode and broadcast a message-delta describing ammo
 * picked up into one of a weapon's magazines while a reload is in progress. The message carries the weapon's
 * network-stable object index, the magazine, the (unclamped) new loaded-round count, and the clamped number of
 * rounds that actually fit into the magazine's reserve. Sent to every loaded machine.
 *
 * DEVIATION: the decompiler models the contiguous on-stack payload as four separate locals (one int followed by
 * three __int16, at stack offsets +0x50/+0x54/+0x56/+0x58); the encoder reads the whole payload starting at the
 * int. Reproduced as four adjacent locals so the call shape matches, as in player_spawn_to_network. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/message_definition.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);

extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void weapon_ammo_pickup_mid_reload_to_network(int weapon_index, int16_t magazine_index, int16_t rounds_picked_up)
{
    int message_weapon_index;
    __int16 message_magazine_index;
    __int16 message_new_loaded;
    __int16 message_reserve_delta;

    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_magazine *magazine = &weapon->weapon.magazines[magazine_index];

    message_magazine_index = magazine_index;
    message_weapon_index = field_translated_index_translate_index(&field_properties_object_index_definition,
                               weapon_index);
    message_new_loaded = magazine->rounds_total + rounds_picked_up;

    __int16 loaded_rounds = magazine->rounds_total;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_magazine_definition *magazine_definition =
        &((weapon_magazine_definition *)definition->weapon.magazines.address)[magazine_index];
    __int16 magazine_loaded_capacity = magazine_definition->rounds_reloaded;
    __int16 total_capacity = magazine_definition->rounds_loaded_maximum;
    __int16 reserve_rounds = magazine->rounds_loaded;

    /* clamp the current loaded rounds to the magazine's loaded capacity for the old-total baseline */
    __int16 clamped_loaded = loaded_rounds;
    if ( magazine_loaded_capacity <= loaded_rounds )
        clamped_loaded = magazine_loaded_capacity;

    int old_total = (__int16)(reserve_rounds + clamped_loaded);
    if ( old_total <= total_capacity )
    {
        __int16 new_loaded = loaded_rounds + rounds_picked_up;
        if ( magazine_loaded_capacity <= new_loaded )
            new_loaded = magazine_loaded_capacity;
        int new_total = (__int16)(reserve_rounds + new_loaded);
        if ( new_total > total_capacity )
            new_total = total_capacity;
        message_reserve_delta = new_total - old_total;
    }
    else
    {
        message_reserve_delta = 0;
    }

    int encoded_size_in_bits = message_delta_processor_encode_stateless(_message_weapon_ammo_pickup_mid_reload, 0,
                                   &message_weapon_index, g_message_encode_buffer, 0x7FF8);
    network_game_server *server = global_network_game_server_get();
    network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
        g_message_encode_buffer, encoded_size_in_bits, 1u, 0, 0, 3);
}
