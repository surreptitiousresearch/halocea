/* game_all_quiet @0x83688350 — return true when the battlefield is fully quiet: no dangerous projectiles,
 * items, or effects near the player, no dangerous units, and no enemies that can see the player. */

#include <stdint.h>

extern uint8_t dangerous_projectiles_near_player(void);
extern uint8_t dangerous_items_near_player(void);
extern uint8_t dangerous_effects_near_player(void);
extern uint8_t any_unit_is_dangerous(void);
extern uint8_t ai_enemies_can_see_player(void);

uint8_t game_all_quiet(void)
{
    if ( dangerous_projectiles_near_player() )
        return 0;
    if ( dangerous_items_near_player() )
        return 0;
    if ( dangerous_effects_near_player() )
        return 0;
    if ( any_unit_is_dangerous() )
        return 0;
    if ( ai_enemies_can_see_player() )
        return 0;
    return 1;
}
