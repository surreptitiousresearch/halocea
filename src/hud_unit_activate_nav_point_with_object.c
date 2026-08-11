/* hud_unit_activate_nav_point_with_object @0x837EE0C8 — activate an object-anchored HUD nav point for
 * the player controlling the given unit (no-op if the unit isn't player-controlled).
 *
 * DEVIATION: vertical_offset arrives as a double via the soft-float ABI; it is a float. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern int player_index_from_unit_index(int unit_index);
extern void hud_activate_nav_point(int16_t nav_index, int player_index, int16_t type, int reference_index, float vertical_offset);

void hud_unit_activate_nav_point_with_object(int16_t nav_index, int unit_index, int object_index, float vertical_offset)
{
    int player_index = player_index_from_unit_index(unit_index);
    if ( player_index != -1 )
        hud_activate_nav_point(nav_index, player_index, _nav_point_object, object_index, vertical_offset);
}
