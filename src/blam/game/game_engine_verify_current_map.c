/* game_engine_verify_current_map @0x8374E860 — release-stripped verification stub. Disasm shows 6
 * back-to-back calls to netgame_verify_spawn_points with no register setup between them (all 4 argument
 * registers are left untouched across calls), so all 6 calls pass through the same arguments unchanged;
 * decompiler's own locals for calls 2-6 are uninitialized garbage and were discarded. Matches the
 * release-stripped assert-loop pattern seen in netgame_flag_verify_team_range / netgame_verify_equipment. */

#include <stdint.h>

extern void netgame_verify_spawn_points(int16_t game_engine, int16_t team, int16_t count, char *string);

void game_engine_verify_current_map(int16_t game_engine, int16_t team, int16_t count, char *string)
{
    netgame_verify_spawn_points(game_engine, team, count, string);
    netgame_verify_spawn_points(game_engine, team, count, string);
    netgame_verify_spawn_points(game_engine, team, count, string);
    netgame_verify_spawn_points(game_engine, team, count, string);
    netgame_verify_spawn_points(game_engine, team, count, string);
    netgame_verify_spawn_points(game_engine, team, count, string);
}
