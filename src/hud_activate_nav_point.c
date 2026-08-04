/* hud_activate_nav_point @0x837ECF70 — activates (or refreshes) a HUD nav point for a player. If a slot
 * already targets the same type+reference it is just updated in place; otherwise the function fills the
 * last deactivated slot (type nibble 0xF) with the new nav point. No-op if the player has no nav table
 * (local index > 1) or the inputs are invalid. */

#include <stdint.h>
#include "headers/hud_nav_point_datum.h"
#include "headers/hud_nav_point_type.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void hud_activate_nav_point(int16_t nav_index, int player_index, int16_t type, int reference_index, float vertical_offset)
{
    if (player_index == -1)
        return;

    /* the player datum's local/controller index lives at +2; record stride is 512 bytes */
    unsigned int local_player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
    if (local_player > 1 || reference_index == -1 || nav_index == -1)
        return;

    hud_nav_point_player_datum *player_navs = &nav_point_data[local_player];

    __int16 free_slot = -1;
    int slot = 0;
    do
    {
        hud_nav_point_datum *nav = &player_navs->nav_points[slot];
        /* recovered: (int)(__int16)nav->packing >> 12 -> nav->type (signed 4-bit; 0xF deactivated == -1) */
        int slot_type = nav->type;
        if (slot_type == type && nav->reference_index == reference_index)
        {
            nav->z_offset = vertical_offset;
            nav->nav_index = nav_index;
            return;
        }
        if (slot_type == -1)
            free_slot = (__int16)slot;
        slot = (__int16)(slot + 1);
    } while (slot < MAXIMUM_ACTIVE_NAV_POINTS);

    if (free_slot != -1)
    {
        hud_nav_point_datum *nav = &player_navs->nav_points[free_slot];
        nav->z_offset = vertical_offset;
        nav->reference_index = reference_index;
        nav->nav_index = nav_index;
        /* recovered: nav->packing = (type << 12) | (old_packing & 0xFFF) -> nav->type = type */
        nav->type = type;
    }
}
