/* _rasterizer_present @0x8369C338 — present the rendered frame unless the D3D device is currently lost.
 * The Blam-level interface keeps the legacy 2-arg screenshot signature — both callers maintain it
 * (halt_and_catch_fire passes li r3,0/li r4,0; render_frame_present swaps r3<->r4 before its tail call) —
 * but the binary's tail `b hcex_render_present` reaches the HCEX bridge hook, which is void(void) and
 * clobbers r3 before any read (disasm 0x823F0F48: first r3 use is a load of d3dDriver). The screenshot
 * args are therefore dead beyond this thunk in the HCEX build; the previous 2-arg extern for the bridge
 * was wrong (it belonged to the legacy screenshot-capable present, not the hook). */
typedef struct bitmap_data bitmap_data;
#include "headers/point2d.h"
#include "headers/blam_data_globals.h"
extern void hcex_render_present(void);
void _rasterizer_present(bitmap_data *screenshot_bitmap, const point2d *screenshot_index)
{
    (void)screenshot_bitmap;  /* dead in HCEX: the bridge present hook takes no args */
    (void)screenshot_index;
    if ( !device_is_lost )
        hcex_render_present();
}
