/* create_the_ball @0x83816F48 — spawn a physical oddball object, unless the variant's ball type is "magic"
 * or "terminator" (those modes track the oddball without a real object). Places it at
 * find_position_for_ball's chosen spot, tags it with owner_team_index = ball_index (the ball's own slot
 * index, reused as a team-index-shaped field), and disables automatic deactivation so it never despawns.
 *
 * DEVIATION: the decompiler's condition (`oddball_ball_type <= _oddball_normal || oddball_ball_type >
 * _oddball_terminator` gates skipping) reduces, once the short-circuit/comma-operator side effects are
 * traced through, to "skip only for _oddball_magic or _oddball_terminator" — rewritten as the equivalent
 * direct check. */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/object_placement_data.h"

extern game_variant *game_engine_get_variant(void);
extern int get_ball_definition_index(void);
extern real_point3d * find_position_for_ball(real_point3d *result, int ball_index);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);

void create_the_ball(int ball_index)
{
    oddball_ball_type ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    if ( ball_type == _oddball_magic || ball_type == _oddball_terminator )
        return;

    int ball_definition_index = get_ball_definition_index();
    if ( ball_definition_index == -1 )
        return;

    object_placement_data placement;
    object_placement_data_new(&placement, ball_definition_index, -1);
    placement.owner_team_index = ball_index;

    real_point3d position;
    find_position_for_ball(&position, ball_index);
    placement.position = position;

    int object_index = object_new(&placement);
    object_set_automatic_deactivation(object_index, 0);
}
