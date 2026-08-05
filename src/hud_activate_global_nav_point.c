/* hud_activate_global_nav_point @0x837ED258 — like hud_activate_nav_point, but applies to every in-use
 * player rather than a single one: walks player_data via a data_iterator and activates/refreshes the nav
 * point slot for each player that has a valid local/controller index (0 or 1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hud_nav_point_datum.h"
#include "headers/hud_nav_point_type.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void hud_activate_global_nav_point(int16_t nav_index, int16_t type, int reference_index, float vertical_offset)
{
    data_iterator it;
    data_iterator_new(&it, player_data);

    for (player_datum *player = data_iterator_next(&it); player;
         player = data_iterator_next(&it))
    {
        /* sentinel: local_player_index is __int16; keep the unsigned compare against 0xFFFF */
        if ((uint16_t)player->local_player_index == 0xFFFF || it.index == -1)
            continue;

        /* the player datum's local/controller index (record stride 512 bytes) */
        unsigned int local_player = DATA_ARRAY_ELEMENT(player_data, player_datum, it.index)->local_player_index;
        if (local_player > 1 || reference_index == -1 || nav_index == -1)
            continue;

        hud_nav_point_player_datum *player_navs = &nav_point_data[local_player];

        int16_t free_slot = -1;
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
                goto next_player;
            }
            if (slot_type == -1)
                free_slot = (int16_t)slot;
            slot = (int16_t)(slot + 1);
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

    next_player:;
    }
}
