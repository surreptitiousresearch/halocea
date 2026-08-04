/* hud_picked_up_powerup @0x83708934 — show the "picked up powerup" HUD message (server path) and fire
 * the matching script event for overshield (powerup type 2) or health (type 5). The powerup type is a
 * word at the equipment definition +776. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/equipment_definition.h"
#include "headers/equipment_powerup_type.h"

extern void hcex_fire_plr_event(const char *eventName, int plr_idx);
extern void hud_add_item_message_server(int16_t local_player_index, int8_t machine_index, int item_definition_index, int16_t quantity, uint8_t message_offset);

void hud_picked_up_powerup(int16_t local_player_index, int8_t machine_index, int powerup_definition_index)
{
    if ( local_player_index != -1 && powerup_definition_index != -1 )
    {
        equipment_definition *powerup =
            TAG_GET(equipment_definition, powerup_definition_index);
        if ( (unsigned short)powerup->equipment.powerup_type == _powerup_type_over_shield )
            hcex_fire_plr_event("picked_up_over_shield", local_player_index);
        if ( (unsigned short)powerup->equipment.powerup_type == _powerup_type_health )
            hcex_fire_plr_event("picked_up_health", local_player_index);
    }
    hud_add_item_message_server(local_player_index, machine_index, powerup_definition_index, 0, 0);
}
