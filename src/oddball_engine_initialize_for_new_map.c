/* oddball_engine_initialize_for_new_map @0x83816FE0 — reset Oddball state for a new map: zero
 * oddball_globals/oddball_globals_baseline, seed score_to_win from the variant (x1800 unless the ball type
 * is terminator), touch-check both teams' oddball flags, and fill current_ball_owner[0..15] with -1. On the
 * server (game_connection() == _game_connection_network_server): for magic/terminator ball types, zero individual_score[0..15] and spawn
 * ball_spawn_count balls (each via create_the_ball); otherwise, seed individual_score[i] with a running
 * 450-per-ball total (juggernaut-style staggered scoring) up to ball_spawn_count entries. Always returns 1.
 *
 * DEVIATION: the score_to_win multiplier condition (`(X-2) != (X-3) + is_terminator`, where X is an
 * unrelated byte read from `terminator+16`) algebraically cancels X entirely — reduces to exactly
 * `!is_terminator` (verified by hand-substitution, not assumed). The "is magic or terminator" flag (`v8`)
 * is a similarly compiled comparison-with-shortcut chain, reduced to the equivalent range check. The two
 * `find_netgame_flag` calls (team 0 and team 1, type 2, return values unused) pass literal 0.0/0.0/2 per
 * disasm, not the decompiler's uninitialized-looking locals. The `current_ball_owner`/`individual_score`
 * fill loops are the walking-struct-pointer idiom seen elsewhere this session, restored to plain indexing. */

#include <stdint.h>
#include "headers/oddball_globals.h"
#include "headers/game_variant.h"
#include "headers/oddball_ball_type.h"
#include "headers/real_point3d.h"
#include "headers/game_connection.h"
#include "headers/netgame_flag_type.h"
#include "headers/blam_data_globals.h"


extern void *memset(void *destination, int value, unsigned int size);
extern game_variant *game_engine_get_variant(void);
extern int find_netgame_flag(real_point3d *location, float distance, float height_delta, int16_t type, int16_t team);
extern int16_t game_connection(void);
extern void create_the_ball(int ball_index);

uint8_t oddball_engine_initialize_for_new_map(void)
{
    memset(&oddball_globals, 0, sizeof(oddball_globals));
    memset(&oddball_globals_baseline, 0, sizeof(oddball_globals_baseline));

    oddball_globals.score_to_win = game_engine_get_variant()->universal_variant.score_to_win;
    if ( game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type != _oddball_terminator )
        oddball_globals.score_to_win *= 1800;

    find_netgame_flag(nullptr, 0.0f, 0.0f, _netgame_flag_oddball_ball_spawn, 0);
    find_netgame_flag(nullptr, 0.0f, 0.0f, _netgame_flag_oddball_ball_spawn, 1);

    for ( int i = 0; i < 16; ++i )
        oddball_globals.current_ball_owner[i] = -1;

    if ( game_connection() != _game_connection_network_server )
        return 1;

    oddball_ball_type oddball_ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    uint8_t spawn_multiple_balls = oddball_ball_type > _oddball_normal && oddball_ball_type <= _oddball_terminator;

    if ( spawn_multiple_balls )
    {
        int ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
        for ( int i = 0; i < ball_spawn_count; ++i )
        {
            oddball_globals.individual_score[i] = 0;
            create_the_ball(i);
        }
        return 1;
    }

    int ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
    int running_total = 0;
    for ( int i = 0; i < ball_spawn_count; ++i )
    {
        running_total += 450;
        oddball_globals.individual_score[i] = running_total;
    }

    return 1;
}
