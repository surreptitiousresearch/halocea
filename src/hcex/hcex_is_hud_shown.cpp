/* hcex_is_hud_shown @0x83683950 — should the HUD be drawn for the player currently being rendered? True only
 * when the scripted show_hud flag is set, the rendered local player maps to a valid player with a live unit
 * (player_datum.unit_index != -1), and the director perspective is neither scripted (2) nor neutral (3). */

#include <stdint.h>
#include "../headers/hud_scripted_globals.h"
#include "../headers/render_globals.h"
#include "../headers/data_array.h"
#include "../headers/player_datum.h"
#include "../headers/director_perspective.h"

extern "C" data_array    *player_data;
extern "C" render_globals render;
extern "C" int local_player_get_player_index(int16_t local_player_index);
extern "C" int16_t director_get_perspective(int16_t local_player_index);

extern "C" int hcex_is_hud_shown(void)
{
    if ( !hud_scripted_globals->show_hud )
        return 0;

    int player_index = local_player_get_player_index(render.local_player_index);
    if ( player_index == -1 )
        return 0;

    /* the decompiler's `if (data + offset)` null-test here is a dead artifact (never null); the real gate is
     * the unit object index (player_datum.unit_index) being valid. */
    if ( DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index == -1 )
        return 0;

    int perspective = director_get_perspective(render.local_player_index);
    if ( perspective == _director_perspective_neutral )
        return 0;
    if ( perspective == _director_perspective_scripted )
        return 0;
    return 1;
}
