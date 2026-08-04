/* weapon_ammo_pickup_mid_reload_from_network @0x836D8910 — (client) apply a replicated "ammo picked up
 * mid-reload" message-delta: resolve the network object index to a local weapon (type mask 4), and if its
 * state byte at magazine-array offset +344 (magazine stride 12, array base +688 in the weapon object) is
 * 1, directly set a separate per-magazine loaded-rounds mirror at object offset 732 (word stride 2);
 * otherwise add the delta to the reserve-rounds word at magazine offset +348. Counterpart of
 * weapon_ammo_pickup_mid_reload_to_network. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/field_properties_definition.h"
#include "headers/weapon_datum.h"
#include "headers/networked_datum_role.h"
#include "headers/weapon_magazine_state.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);

void weapon_ammo_pickup_mid_reload_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    struct
    {
        int    translated_weapon_index;
        __int16 magazine_index;
        __int16 new_loaded;
        __int16 rounds_picked_up;
    } payload;

    if ( !message_delta_processor_decode_stateless(&payload, header) )
        return;

    int weapon_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                           payload.translated_weapon_index);
    weapon_datum *weapon = object_try_and_get_and_verify_type(weapon_index, object_mask_weapon);
    if ( !weapon )
        return;

    /* datum_role is an opaque 4-byte NetworkedDatumRole at object+0; compare its raw word */
    if ( *(int *)&weapon->object.datum_role != _networked_datum_puppet )
        return;

    weapon_magazine *magazine = &weapon->weapon.magazines[payload.magazine_index];
    if ( magazine->state == _weapon_magazine_reloading )
    {
        weapon->weapon.magazine_rounds_total_to_use_at_reload_finish[payload.magazine_index] =
            payload.new_loaded;
    }
    else
    {
        __int16 reserve_rounds = magazine->rounds_loaded;
        magazine->rounds_total = payload.new_loaded;
        magazine->rounds_loaded = reserve_rounds + payload.rounds_picked_up;
    }
}
