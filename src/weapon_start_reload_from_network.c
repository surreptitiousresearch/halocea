/* weapon_start_reload_from_network @0x836D9B20 — (client only) apply a "weapon start reload" message-delta:
 * resolve the network object index to a local weapon, and if it is in ready state, store the replicated
 * loaded/reserve round counts into the weapon's desired-rounds mirror arrays and flag the pending-network-
 * reload bit.
 *
 * Disasm-verified offsets (weapon_datum): the message's counts land in the per-magazine
 * weapon.magazine_rounds_total_at_reload_start[mag] (+724) and weapon.magazine_rounds_loaded_at_reload_start[mag]
 * (+728) arrays — deliberately NOT the 12-byte-stride weapon.magazines[] state fields the server encodes from
 * (+694/+696); object.datum_role (+4) must be _networked_datum_puppet, and weapon.flags (+556) gets bit 3
 * (0x8: network reload pending) OR'd in. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_start_reload_message.h"
#include "headers/weapon_datum_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void weapon_start_reload_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    weapon_start_reload_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    int weapon_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                                                              message.weapon_index);
    weapon_datum *weapon = object_try_and_get_and_verify_type(weapon_index, object_mask_weapon);
    if ( weapon && weapon->object.datum_role == _networked_datum_puppet )
    {
        weapon->weapon.magazine_rounds_total_at_reload_start[message.magazine_index] = message.starting_total_rounds;
        weapon->weapon.magazine_rounds_loaded_at_reload_start[message.magazine_index] = message.starting_loaded_rounds;
        weapon->weapon.flags |= (1u << _weapon_needs_to_reload_bit); /* bit 3: network reload pending */
    }
}
