/* race_engine_initialize_for_new_map @0x8382B8A8 — reset the Race game-engine state for a new map:
 * de-duplicate script-placed race flags, zero race_globals/race_globals_baseline, reset the endgame-sound
 * timeout, then scan every scenario netgame flag of type 3 (race checkpoint) to register it as a
 * multiplayer goal marker (position, "flag_blue" nav point, no owning player/team) and OR its index into
 * lap_completed_value's bit vector, tracking the lowest checkpoint index seen (capped at 32).
 *
 * If the variant is rally mode (terminator.ignored == 2), spawns a fresh rally flag and returns. Otherwise,
 * fills race_globals.first_flag[0..15] with either -1 (terminator.ignored nonzero: no per-flag lap
 * requirement) or the lowest checkpoint index found above (terminator.ignored == 0: shared requirement).
 * Always returns 1.
 *
 * DEVIATION: `game_engine_set_goal_position`'s own established deviation note applies verbatim here — this
 * ABI shadows a GPR slot for the float `vertical_offset` param, so the decompiler mislabeled every
 * subsequent argument one slot early (`descriptor` shown as unused/never-assigned `v3`, "player_index"
 * shown fed the "flag_blue" string, etc). Reattributed per that file's confirmed mapping:
 * (index, position, vertical_offset, descriptor="flag_blue", player_index=-1, team_index=-1,
 * ignore_player_index=-1). The `first_flag[0..15] = ...` fills are rendered as a walking-struct-pointer
 * idiom (`v2 = (race_globals_type*)((char*)v2+4); v2->lap_completed_value = ...`) — same class as the
 * team_score/individual_score walking-pointer loop in oddball_engine_replicate_game_mode_state_to_network.c
 * — restored to plain array indexing. `scenario_netgame_flag.team_index` is reused here as a race
 * checkpoint index, not a player team. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/race_globals.h"
#include "headers/game_variant.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void *memset(void *destination, int value, unsigned int size);
extern int race_flags_make_unique(void);
extern game_variant *game_engine_get_variant(void);
extern void game_engine_set_goal_position(int16_t index, real_point3d *position, float vertical_offset, char *descriptor, int player_index, int16_t team_index, int ignore_player_index);
extern int new_rally_flag(int old_flag);

unsigned __int8 race_engine_initialize_for_new_map(void)
{
    race_flags_make_unique();

    memset(&race_globals, 0, sizeof(race_globals));
    memset(&race_globals_baseline, 0, sizeof(race_globals_baseline));
    timeout_for_endgame_sound = 30;

    int lowest_checkpoint_index = 32;

    for ( int i = 0; i < global_scenario->netgame_flags.count; ++i )
    {
        /* recovered: (char*)address + 148*i -> typed array index */
        scenario_netgame_flag *flag =
            &((scenario_netgame_flag *)global_scenario->netgame_flags.address)[i];

        if ( flag->type != _netgame_flag_race_track )
            continue;

        __int16 checkpoint_index = flag->team_index;
        if ( checkpoint_index >= 32 )
            continue;

        if ( checkpoint_index < lowest_checkpoint_index )
            lowest_checkpoint_index = checkpoint_index;

        race_globals.lap_completed_value |= 1 << checkpoint_index;
        game_engine_set_goal_position(checkpoint_index, &flag->position, 0.0f, "flag_blue", -1, -1, -1);
    }

    if ( game_engine_get_variant()->game_engine_variant.terminator.ignored == 2 )
    {
        race_globals.rally_flag = new_rally_flag(-1);
        return 1;
    }

    int fill_value = game_engine_get_variant()->game_engine_variant.terminator.ignored ? -1 : lowest_checkpoint_index;
    for ( int i = 0; i < 16; ++i )
        race_globals.first_flag[i] = fill_value;

    return 1;
}
