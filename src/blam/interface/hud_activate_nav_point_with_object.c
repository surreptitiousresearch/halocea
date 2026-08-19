/* hud_activate_nav_point_with_object @0x837ED098 — tail-call forwarder (`mr r6, r5; li r5, 1; b
 * hud_activate_nav_point`; confirmed via disasm, not a bare blr). Forwards with type=1 (object) and
 * object_index passed through unchanged as reference_index. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_activate_nav_point(int16_t nav_index, int player_index, int16_t type, int reference_index, float vertical_offset);

void hud_activate_nav_point_with_object(int16_t nav_index, int player_index, int object_index, float vertical_offset)
{
    hud_activate_nav_point(nav_index, player_index, _nav_point_object, object_index, vertical_offset);
}
