/* cheat_all_powerups @0x8369A338 — spawn one of each powerup listed in the game globals' cheat_powerups
 * block in an arc in front of the acting player. */

#include <stdint.h>
#include "headers/game_globals_definition.h"
#include "headers/game_globals_tag_reference.h"
#include "headers/blam_data_globals.h"

extern void cheat_objects(const game_globals_tag_reference *references, int16_t name_count);

void cheat_all_powerups(void)
{
    int count = global_game_globals->cheat_powerups.count;
    const game_globals_tag_reference *references =
        count ? (const game_globals_tag_reference *)global_game_globals->cheat_powerups.address : nullptr;
    cheat_objects(references, count);
}
