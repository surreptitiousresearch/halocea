/* hcex_is_split_screen @0x836A3C28 — true when the game is running local split-screen: more than one local
 * player AND no dedicated co-op local-player override selected (hcex_coop_local_player_index < 0). When a
 * specific co-op local player is pinned (>= 0), rendering is single-view, so this returns false. */

#include <stdint.h>

extern int16_t local_player_count(void);
extern int   hcex_coop_local_player_index;

extern "C" int hcex_is_split_screen(void)
{
    if (local_player_count() <= 1)
        return 0;
    if (hcex_coop_local_player_index >= 0)
        return 0;
    return 1;
}
