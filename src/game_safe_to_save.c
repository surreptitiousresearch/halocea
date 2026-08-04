#include <stdint.h>
#include "headers/blam_data_globals.h"
/* game_safe_to_save @0x836883C8 — return true when it is safe to make a checkpoint: no enemies can see the
 * player, no dangerous projectiles/items/effects nearby, no dangerous units, no player airborne, dead, and
 * no vehicle moving near a player. When debug_game_save is set, the first failing condition is logged. */

extern void console_warning(const char *format, ...);
extern uint8_t ai_enemies_can_see_player(void);
extern uint8_t dangerous_projectiles_near_player(void);
extern uint8_t dangerous_items_near_player(void);
extern uint8_t dangerous_effects_near_player(void);
extern uint8_t any_unit_is_dangerous(void);
extern uint8_t any_player_is_in_the_air(void);
extern uint8_t any_player_is_dead(void);
extern uint8_t vehicle_moving_near_any_player(void);

uint8_t game_safe_to_save(void)
{
    int safe = 1;

    if ( ai_enemies_can_see_player() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: ai_enemies_can_see_player");
        safe = 0;
    }
    if ( safe && dangerous_projectiles_near_player() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: dangerous_projectiles_near_player");
        safe = 0;
    }
    if ( safe && dangerous_items_near_player() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: dangerous_items_near_player");
        safe = 0;
    }
    if ( safe && dangerous_effects_near_player() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: dangerous_effects_near_player");
        safe = 0;
    }
    if ( safe && any_unit_is_dangerous() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: any_unit_is_dangerous");
        safe = 0;
    }
    if ( safe && any_player_is_in_the_air() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: any_player_is_in_the_air");
        safe = 0;
    }
    if ( safe && any_player_is_dead() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: any_player_is_dead");
        safe = 0;
    }
    if ( safe && vehicle_moving_near_any_player() )
    {
        if ( debug_game_save )
            console_warning("not safe to save: vehicle_moving_near_any_player");
        return 0;
    }
    return safe;
}
