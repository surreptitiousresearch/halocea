/* hud_deactivate_nav_point @0x837ED3D8 — deactivate the HUD nav-point of a given type that points at a
 * given reference object/index, for the player's local screen. Searches the local player's 4 nav-point
 * slots; on a match clears its nav_index/reference_index and sets the type nibble to 0xF (invalid). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/hud_nav_point_datum.h"
#include "headers/hud_nav_point_type.h"
#include "headers/blam_data_globals.h"


void hud_deactivate_nav_point(int player_index, int16_t type, int reference_index)
{
    if ( player_index != -1 )
    {
        int16_t local_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
        if ( local_player_index >= 0 && local_player_index < 2 && reference_index != -1 )
        {
            hud_nav_point_player_datum *player_nav = &nav_point_data[local_player_index];
            for ( int i = 0; i < MAXIMUM_ACTIVE_NAV_POINTS; i = (int16_t)(i + 1) )
            {
                hud_nav_point_datum *nav = &player_nav->nav_points[i];
                /* recovered: (__int16 packing = nav->packing; packing >> 12) -> nav->type */
                if ( nav->type == type && nav->reference_index == reference_index )
                {
                    nav->reference_index = -1;
                    nav->nav_index = -1;
                    /* recovered: nav->packing |= 0xF000 -> nav->type = -1 (deactivated nibble 0xF) */
                    nav->type = -1;
                    return;
                }
            }
        }
    }
}
