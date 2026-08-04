/* hud_update @ 0x83707EC0 — advance the heads-up display: weapon, unit, nav-point and messaging
 * sub-updates. When the game engine forces a single (shared) screen, each local player's unit sounds
 * are still played individually.
 *
 * Deviation: the decompiler threaded the sub-update returns through the next call's argument register
 * (PPC reuse artifact); they take no argument. Player datum stride is 512 bytes. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern void hud_update_weapon(void);
extern void hud_update_unit(void);
extern void hud_update_nav_points(void);
extern void hud_messaging_globals_update(void);
extern uint8_t game_engine_force_single_screen(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern void hud_play_unit_sounds(const player_datum *player, uint8_t show_hud);

void hud_update(void)
{
    hud_update_weapon();
    hud_update_unit();
    hud_update_nav_points();
    hud_messaging_globals_update();

    if ( game_engine_force_single_screen() )
    {
        __int16 local_player;
        for ( local_player = 0; local_player < 2; local_player++ )
        {
            int player_index = local_player_get_player_index(local_player);
            if ( player_index != -1 )
                hud_play_unit_sounds(
                    DATA_ARRAY_ELEMENT(player_data, player_datum, player_index), 0);
        }
    }
}
