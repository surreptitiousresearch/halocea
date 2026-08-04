/* hud_deactivate_nav_point_with_object @0x837ED4B8 — tail-call forwarder (`mr r5, r4; li r4, 1; b
 * hud_deactivate_nav_point`; confirmed via disasm, not a bare blr). Forwards with type=1 (object) and
 * object_index passed through unchanged as reference_index. */

#include <stdint.h>
#include "headers/hud_nav_point_type.h"

extern void hud_deactivate_nav_point(int player_index, int16_t type, int reference_index);

void hud_deactivate_nav_point_with_object(int player_unit_index, int object_index)
{
    hud_deactivate_nav_point(player_unit_index, _nav_point_object, object_index);
}
