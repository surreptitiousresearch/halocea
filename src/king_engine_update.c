/* king_engine_update @0x8382C590 — per-tick King of the Hill update. When the game can score, is a live
 * server connection, the variant has assault-style hill rotation enabled, and the hill timer (decremented
 * every tick) reaches 0: resets the timer to 1800 and searches for a new hill (a random start index via
 * seed_random_range, then linearly scanning king_engine_hills[] for one different from the current
 * hill_id), calling find_hill on whichever index is chosen, playing the "new hill" sound, and repeating
 * the search (without resetting the timer) if the new hill turned out to have no boundary points and the
 * previous hill_id was already 0 (nothing to fall back to). Once a hill with boundary points exists (either
 * freshly found or already active), registers it as multiplayer goal 0 ("crown_blue"); otherwise logs
 * "FAILED TO FIND HILL". Finally re-evaluates the hill state on the server.
 *
 * DEVIATION: `find_hill`'s real signature takes 3 args (`int, __int16, __int16`); disasm confirms only the
 * first (the chosen hill index) is ever set at this call site — the other two are read from whatever the
 * preceding search-loop's registers last held, not meaningful arguments, so they're passed as 0/0 here
 * (find_hill itself is a separate, not-yet-reconstructed function). `game_engine_set_goal_position`'s own
 * established ABI-shadow deviation (float `vertical_offset` consumes a GPR slot) applies verbatim — fixed
 * per that file's confirmed mapping (index=0, position=&hill_center, vertical_offset=0.0,
 * descriptor="crown_blue", player_index/team_index/ignore_player_index=-1). The fallback hill id (an
 * unnamed raw decompiler local) is read from a stack slot that's never written on the loop's first pass — reproduced
 * as a genuinely uninitialized local per the "don't invent" doctrine; it only matters if the search loop's
 * first iteration fails to find a different hill AND the previous hill_id was already 0, at which point the
 * loop re-enters and a later iteration's assignment overwrites it before it's used meaningfully. */

#include <stdint.h>
#include "headers/king_globals.h"
#include "headers/game_variant.h"
#include "headers/real_point3d.h"
#include "headers/game_connection.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"

extern int16_t king_engine_hill_count;

extern uint8_t game_engine_can_score(void);
extern int16_t game_connection(void);
extern game_variant *game_engine_get_variant(void);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void find_hill(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void console_printf(uint8_t clear, const char *format, ...);
extern void game_engine_set_goal_position(int16_t index, real_point3d *position, float vertical_offset, char *descriptor, int player_index, int16_t team_index, int ignore_player_index);
extern void king_calculate_hill_state(void);

void king_engine_update(void)
{
    int new_hill_id;
    uint8_t has_hill_points;

    if ( game_engine_can_score() && game_connection() == _game_connection_network_server && game_engine_get_variant()->game_engine_variant.ctf.assault
      && --king_globals.hill_timer == 0 )
    {
        int hill_id = king_globals.hill_id;
        king_globals.hill_timer = 1800;

        for ( ;; )
        {
            int16_t hill_count = king_engine_hill_count;
            int16_t start_index = seed_random_range(get_global_random_seed_address(), 0, hill_count);

            if ( hill_count > 0 )
            {
                for ( int16_t offset = 0; offset < hill_count; offset = (int16_t)(offset + 1) )
                {
                    int16_t candidate = king_engine_hills[(int16_t)((start_index + offset) % hill_count)];
                    if ( hill_id != candidate )
                    {
                        new_hill_id = candidate;
                        break;
                    }
                }
            }

            king_globals.hill_id = new_hill_id;
            find_hill();
            game_engine_play_multiplayer_sound(_multiplayer_sound_hill_move, 1u);

            has_hill_points = king_globals.hill_point_count > 0;
            if ( king_globals.hill_point_count )
                break;

            hill_id = king_globals.hill_id;
            if ( !king_globals.hill_id )
                goto failed_to_find_hill;
        }
    }
    else
    {
        has_hill_points = king_globals.hill_point_count > 0;
    }

    if ( has_hill_points )
    {
        real_point3d hill_center = king_globals.hill_center;
        game_engine_set_goal_position(0, &hill_center, 0.0f, "crown_blue", -1, -1, -1);
    }
    else
    {
failed_to_find_hill:
        console_printf(0, "FAILED TO FIND HILL");
    }

    if ( game_connection() == _game_connection_network_server )
        king_calculate_hill_state();
}
