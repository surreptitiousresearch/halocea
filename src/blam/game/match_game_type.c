/* match_game_type @0x8374CD68 — does the active game engine type satisfy any entry in a game-type list?
 * Each list entry is either a concrete engine type (matched directly) or one of three category codes:
 * 12 matches all types; 13 matches every type except ctf; 14 matches every type except ctf and race. With no
 * game engine (single-player), the list matches only if every entry is 0. Returns nonzero on a match. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/game_engine_type.h"
#include "headers/game_matching_option.h"

/* attested uint8_t: 4/4 callers clrlwi r3,24 */
uint8_t match_game_type(int game_engine_type, int count, const int16_t *game_type)
{
    int result;
    if (game_engine)
    {
        result = 0;
        for (int i = 0; i < count; i++)
        {
            int16_t entry = game_type[i];
            result |= (game_engine_type == entry);
            switch (entry)
            {
                case _game_engine_all:
                    result = (uint8_t)result | 1;
                    break;
                case _game_engine_all_non_team:
                    result |= (game_engine_type != game_engine_ctf);
                    break;
                case _game_engine_all_normal:
                {
                    char matches = 1;
                    if (game_engine_type == game_engine_ctf || game_engine_type == game_engine_race)
                        matches = 0;
                    result = (uint8_t)(result | matches);
                    break;
                }
            }
        }
    }
    else
    {
        result = 1;
        for (int i = 0; i < count; i++)
            result = (game_type[i] == 0) & (uint8_t)result;
    }
    return (uint8_t)result;
}
