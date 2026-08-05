/* hud_deactivate_team_nav_point @0x837ED4EC — deactivates a nav point matching type+reference for every
 * player on the given team. For each such player it finds the nav slot whose type nibble and reference
 * index match, clears it (reference/nav index -1) and marks its type nibble deactivated (0xF). */

#include <stdint.h>
#include "headers/hud_nav_point_datum.h"
#include "headers/hud_nav_point_type.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"


extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

void hud_deactivate_team_nav_point(int16_t team_index, int16_t type, int reference_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for (player_datum *player = data_iterator_next(&iterator);
         player;
         player = data_iterator_next(&iterator))
    {
        if ((uint16_t)player->local_player_index == 0xFFFF
            || team_index != player->team_index || iterator.index == -1)
            continue;

        /* recovered: *(__int16 *)(player_data->data + datum(iterator.index) + 2) -> player->local_player_index */
        unsigned int local_player = player->local_player_index;
        if (local_player > 1 || reference_index == -1)
            continue;

        hud_nav_point_player_datum *player_navs = &nav_point_data[local_player];
        for (int slot = 0; slot < MAXIMUM_ACTIVE_NAV_POINTS; ++slot)
        {
            hud_nav_point_datum *nav = &player_navs->nav_points[slot];
            /* recovered: (__int16 packing = nav->packing; packing >> 12) -> nav->type */
            if (nav->type == type && nav->reference_index == reference_index)
            {
                nav->reference_index = -1;
                nav->nav_index = -1;
                /* recovered: nav->packing |= 0xF000 -> nav->type = -1 (deactivated nibble 0xF) */
                nav->type = -1;
                break;
            }
        }
    }
}
