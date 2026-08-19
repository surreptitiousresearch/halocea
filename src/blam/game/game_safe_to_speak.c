/* game_safe_to_speak @0x836885B8 — return true when it is safe to play scripted dialogue: no dangerous
 * projectiles near the player and no player currently dead. */

#include <stdint.h>

extern uint8_t dangerous_projectiles_near_player(void);
extern uint8_t any_player_is_dead(void);

uint8_t game_safe_to_speak(void)
{
    if ( dangerous_projectiles_near_player() )
        return 0;
    if ( any_player_is_dead() )
        return 0;
    return 1;
}
