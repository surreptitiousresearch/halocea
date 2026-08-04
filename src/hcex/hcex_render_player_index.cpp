/* hcex_render_player_index @0x8369E2F0 — map a requested camera index to the local player index that should
 * actually be rendered in co-op. Passes the index through when there is no split co-op view (single local
 * player, or no co-op local player set). Otherwise, when the co-op local player is in the scripted director
 * perspective (used for dead / transition cameras), forces player 0; else renders the co-op local player. */

#include <stdint.h>
#include "../headers/director_perspective.h"

extern int16_t local_player_count(void);
extern int16_t director_get_perspective(int16_t local_player_index);
extern int     hcex_coop_local_player_index;

extern "C" int hcex_render_player_index(int cam_idx)
{
    if ( cam_idx < 0 || local_player_count() <= 1 || hcex_coop_local_player_index < 0 )
        return cam_idx;

    return director_get_perspective(hcex_coop_local_player_index) == _director_perspective_scripted
               ? 0 : hcex_coop_local_player_index;
}
