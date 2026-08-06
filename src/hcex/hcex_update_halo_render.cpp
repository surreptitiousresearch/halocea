/* hcex_update_halo_render @0x8369E438 — hcex bridge: refresh the Blam render camera for each active view.
 * Updates view 0 always, view 1 only for local (non-remote) split-screen co-op, then issues a final update
 * for the shared/global view (index -1). The per-view work is done by hcex_update_halo_render_cam. */

#include <stdint.h>

extern "C" int hcex_coop_local_player_index;

extern "C" int16_t local_player_count(void);
extern "C" void hcex_update_halo_render_cam(int cam_idx);

extern "C" void hcex_update_halo_render(void)
{
    if ( local_player_count() > 0 )
        hcex_update_halo_render_cam(0);
    if ( local_player_count() > 1 && hcex_coop_local_player_index < 0 )
        hcex_update_halo_render_cam(1);
    hcex_update_halo_render_cam(-1);
}
