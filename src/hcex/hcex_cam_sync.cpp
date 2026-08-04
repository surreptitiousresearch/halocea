/* hcex_cam_sync @0x8369E640 — hcex bridge: push each active local player's camera into the Blam rasterizer
 * for this frame. Always syncs window 0; syncs window 1 only for split-screen co-op that is NOT driven by a
 * remote co-op player (hcex_coop_local_player_index < 0). The per-window work is done by hcex_cam_sync_by_idx. */

#include <stdint.h>

extern int hcex_coop_local_player_index;

extern int16_t local_player_count(void);
extern void hcex_cam_sync_by_idx(int idx);

extern "C" void hcex_cam_sync(void)
{
    if ( local_player_count() > 0 )
        hcex_cam_sync_by_idx(0);
    if ( local_player_count() > 1 && hcex_coop_local_player_index < 0 )
        hcex_cam_sync_by_idx(1);
}
