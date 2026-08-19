/* hud_activate_team_nav_point_with_object @0x837ED248 — thunk activating a team nav point
 * anchored to a game object (object_type 1), forwarding to hud_activate_team_nav_point.
 * DEVIATION: decompiler typed vertical_offset as double (soft-float ABI artifact); it is a float. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_activate_team_nav_point(int16_t nav_index, int16_t team_index, int16_t type, int reference_index, float vertical_offset);

void hud_activate_team_nav_point_with_object(int16_t nav_index, int16_t team_index, int object_index, float vertical_offset)
{
    hud_activate_team_nav_point(nav_index, team_index, _nav_point_object, object_index, vertical_offset);
}
