#include <stdint.h>
#include "headers/game_connection.h"
/* hud_picked_up_weapon @0x83708898 — show the "picked up weapon" HUD item message for a player. Fires
 * the script event, then routes the message through the client path (on clients, or for client-side
 * pickups) or the server path otherwise. */

extern void hcex_fire_plr_event(const char *event_name, int player_identifier); /* DEVIATION: slot 1 is int, not short -- def src/hcex/hcex_fire_plr_event.cpp + prologue stw r4 @0x823E4730 */
extern int16_t game_connection(void);
extern void hud_add_item_message_client(int16_t local_player_index, int item_definition_index, int16_t quantity, char message_offset);
extern void hud_add_item_message_server(int16_t local_player_index, int8_t machine_index, int item_definition_index, int16_t quantity, int8_t message_offset);

void hud_picked_up_weapon(int16_t local_player_index, char machine_index, int weapon_definition_index, uint8_t is_client_side)
{
    if ( local_player_index != -1 )
        hcex_fire_plr_event("picked_up_weapon", local_player_index);
    if ( game_connection() != _game_connection_network_server || is_client_side )
        hud_add_item_message_client(local_player_index, weapon_definition_index, 0, 0);
    else
        hud_add_item_message_server(local_player_index, machine_index, weapon_definition_index, 0, 0);
}
