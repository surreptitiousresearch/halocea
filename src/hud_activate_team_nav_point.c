/* hud_activate_team_nav_point @0x837ED0A8 — activates (or refreshes) a HUD nav point for every local
 * player on a given team. For each such player it updates the matching type+reference slot in place,
 * or fills the last deactivated slot (type nibble 0xF) with the new nav point. Mirrors
 * hud_activate_nav_point's per-player slot logic. */

#include <stdint.h>
#include "headers/hud_nav_point_datum.h"
#include "headers/hud_nav_point_type.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void hud_activate_team_nav_point(int16_t nav_index, int16_t team_index, int16_t type, int reference_index, float vertical_offset)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for (player_datum *player = data_iterator_next(&iterator); player;
         player = data_iterator_next(&iterator))
    {
        if ((uint16_t)player->local_player_index == 0xFFFF || team_index != player->team_index || iterator.index == -1)
            continue;

        unsigned int local_player = DATA_ARRAY_ELEMENT(player_data, player_datum, iterator.index)->local_player_index;
        if (local_player > 1 || reference_index == -1 || nav_index == -1)
            continue;

        hud_nav_point_player_datum *player_navs = &nav_point_data[local_player];

        int16_t free_slot = -1;
        int slot = 0;
        bool updated = false;
        do
        {
            hud_nav_point_datum *nav = &player_navs->nav_points[slot];
            /* recovered: (int)(__int16)nav->packing >> 12 -> nav->type (signed 4-bit; 0xF deactivated == -1) */
            int slot_type = nav->type;
            if (slot_type == type && nav->reference_index == reference_index)
            {
                nav->z_offset = vertical_offset;
                nav->nav_index = nav_index;
                updated = true;
                break;
            }
            if (slot_type == -1)
                free_slot = (int16_t)slot;
            slot = (int16_t)(slot + 1);
        } while (slot < MAXIMUM_ACTIVE_NAV_POINTS);

        if (!updated && free_slot != -1)
        {
            hud_nav_point_datum *nav = &player_navs->nav_points[free_slot];
            nav->z_offset = vertical_offset;
            nav->reference_index = reference_index;
            nav->nav_index = nav_index;
            /* recovered: nav->packing = (type << 12) | (old_packing & 0xFFF) -> nav->type = type */
            nav->type = type;
        }
    }
}
