/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* hud_add_item_message_from_network @0x836A5A78 — (client) apply an "add HUD item message" message-delta:
 * find the first local player (local_player_index != 0xFFFF) and hand the decoded item/quantity/message-
 * offset to hud_add_item_message_client for that player. Then, depending on the item tag's type (offset 0,
 * `__int16`) and "pickup effect" sub-type (offset 776): an over-shield pickup (type 3, sub-type 2) plays
 * the over-shield screen effect and, on a standalone connection, forces bit 0x10 in the player's unit's
 * flags word (the same raw offset 0x106 flags word already documented in unit_kill_from_network.c); an
 * active-camo pickup (sub-type 3) or health-pack pickup (sub-type 5) just plays their screen effect. If the
 * resolved tag carries a pickup sound definition (type 3 offset 796, or type 2 offset 1180), plays it as an
 * unspatialized impulse sound. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/equipment_definition.h"
#include "headers/weapon_definition.h"
#include "headers/hud_add_item_message.h"
#include "headers/game_connection.h"
#include "headers/object_damage_flags.h"
#include "headers/equipment_powerup_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);

extern void hud_add_item_message_client(int16_t local_player_index, int item_definition_index,
        int16_t quantity, char message_offset);
extern int16_t game_connection(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void player_over_shield_screen_effect(int player_index);
extern void player_active_camo_screen_effect(int player_index);
extern void player_health_pack_screen_effect(int player_index);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void hud_add_item_message_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    hud_add_item_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    data_iterator player_iterator;
    data_iterator_new(&player_iterator, player_data);
    player_datum *player = data_iterator_next(&player_iterator);
    if ( !player )
        return;

    while ( player->local_player_index == (int16_t)0xFFFF )
    {
        player = data_iterator_next(&player_iterator);
        if ( !player )
            return;
    }

    hud_add_item_message_client(player->local_player_index, message.item_definition_index, message.quantity,
            message.message_offset);

    /* DEVIATION FIX: restored missing deref — DB loads the slot value (definition ptr), not its address */
    _object_definition *item_definition = TAG_GET(_object_definition, message.item_definition_index);
    int sound_definition_index = -1;

    if ( item_definition->type == object_type_equipment )
    {
        switch ( (uint16_t)((equipment_definition *)item_definition)->equipment.powerup_type )
        {
            case _powerup_type_over_shield:
                player_over_shield_screen_effect(player_iterator.index);
                if ( game_connection() == _game_connection_network_client )
                {
                    unit_datum *unit = object_try_and_get_and_verify_type(player->unit_index, object_mask_biped);
                    if ( unit )
                        unit->object.damage_flags |= (1u << _object_shield_over_charging_bit);
                }
                break;
            case _powerup_type_active_camouflage:
                player_active_camo_screen_effect(player_iterator.index);
                break;
            case _powerup_type_health:
                player_health_pack_screen_effect(player_iterator.index);
                break;
        }
        sound_definition_index = ((equipment_definition *)item_definition)->equipment.pickup_sound.index;
    }
    else if ( item_definition->type == object_type_weapon )
    {
        sound_definition_index = ((weapon_definition *)item_definition)->weapon.pickup_sound.index;
    }

    if ( sound_definition_index != -1 )
        unspatialized_impulse_sound_new(sound_definition_index, 1.0f);
}
