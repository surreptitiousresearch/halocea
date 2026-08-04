/* first_person_weapon_get_marker_by_name_render @0x8369F8E8 — render-time guard around
 * first_person_weapon_get_marker_by_name. Only resolves the named markers when the weapon's owning
 * local player is the one currently being rendered (render.local_player_index); otherwise returns 0
 * (no markers). Keeps marker attachment (e.g. muzzle flashes, effects) scoped to the correct viewport
 * during split-screen rendering. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/object_marker.h"
#include "headers/blam_data_globals.h"


extern int16_t first_person_weapon_index_from_weapon_index(int weapon_index);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

int16_t first_person_weapon_get_marker_by_name_render(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count)
{
    if ( render.local_player_index == first_person_weapon_index_from_weapon_index(weapon_index) )
        return first_person_weapon_get_marker_by_name(weapon_index, name, markers, maximum_marker_count);
    return 0;
}
