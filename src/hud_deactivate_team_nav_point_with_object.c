/* hud_deactivate_team_nav_point_with_object @0x837ED608 — thunk deactivating an object-anchored
 * (_nav_point_object) team nav point, forwarding to hud_deactivate_team_nav_point. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_deactivate_team_nav_point(int16_t team_index, int16_t type, int reference_index);

void hud_deactivate_team_nav_point_with_object(int16_t team_index, int object_index)
{
    hud_deactivate_team_nav_point(team_index, _nav_point_object, object_index);
}
