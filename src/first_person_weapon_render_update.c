/* first_person_weapon_render_update @0x8369F830 — update the local player's first-person weapon visibility
 * and, when visible, rebuild its node matrices for rendering. The viewmodel is hidden whenever the camera
 * is not in the first-person perspective or the player is zoomed in. No-op when no local player is being
 * rendered or the player has no unit/weapon. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/render_globals.h"

extern int16_t director_get_perspective(int16_t local_player_index);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern void first_person_weapon_set_visibility(int16_t local_player_index, uint8_t visible);
extern void first_person_weapon_build_node_matrices(int16_t local_player_index);

void first_person_weapon_render_update(void)
{
    if (render.local_player_index == -1)
        return;

    first_person_weapon *weapon = &first_person_weapons[render.local_player_index];
    if (weapon->unit_index == -1 || weapon->weapon_index == -1)
        return;

    unsigned __int8 visible = 1;
    if (director_get_perspective(render.local_player_index)
        || player_control_get_zoom_level(render.local_player_index) != -1)
        visible = 0;

    first_person_weapon_set_visibility(render.local_player_index, visible);
    if (weapon->visible)
        first_person_weapon_build_node_matrices(render.local_player_index);
}
