/* hcex_cam_sync_by_idx @0x8369E4A0 — hcex bridge: push one render window's camera into the Blam rasterizer
 * via hcex_set_cam. Under debug_no_drawing, pulls directly from the already-computed hcex_window/hcex_fov
 * state (no live observer needed); otherwise resolves the local player's actual observer camera and uses
 * rasterizer_globals for the near/far planes. Both paths convert the stored field-of-view from radians to
 * degrees (RAD_TO_DEG = 180/pi) before handing it to hcex_set_cam.
 *
 * DEVIATION: the raw decompile mixed typed field access (`camera->position.n[2]`, `camera->forward.n[0]`)
 * with raw float-indexed access (`v12[11]`, `v12[13]`, ...) on the very same `observer_result*` for no
 * apparent reason — every raw index cross-checked exactly against the struct's own established layout
 * (`v12[11..13]` = `up.n[0..2]`, `v12[14]` = `field_of_view`, `v12[1]` = `position.n[1]`), so this is
 * rewritten entirely in terms of named fields with no behavioral difference. */

#include <stdint.h>
#include "../headers/render_window.h"
#include "../headers/observer_result.h"
#include "../headers/hcex/hcex_float3.h"
#include "../headers/rasterizer_globals.h"
#include "../headers/math_constants.h"

extern unsigned char debug_no_drawing;
extern render_window hcex_window[];
extern float hcex_fov[];
extern rasterizer_globals_struct rasterizer_globals;

extern const observer_result *observer_get_camera(int16_t local_player_index);
extern void hcex_set_cam(int idx, hcex_float3 *pos, hcex_float3 *up, hcex_float3 *forv, float fov, float z_near,
        float z_far);

extern "C" void hcex_cam_sync_by_idx(int idx)
{
    if ( debug_no_drawing )
    {
        render_window *window = &hcex_window[idx + 1];

        hcex_float3 pos = { window->rasterizer_camera.position.n[0], window->rasterizer_camera.position.n[1],
                window->rasterizer_camera.position.n[2] };
        hcex_float3 forv = { window->rasterizer_camera.forward.n[0], window->rasterizer_camera.forward.n[1],
                window->rasterizer_camera.forward.n[2] };
        hcex_float3 up = { window->rasterizer_camera.up.n[0], window->rasterizer_camera.up.n[1],
                window->rasterizer_camera.up.n[2] };

        hcex_set_cam(idx, &pos, &up, &forv, hcex_fov[idx + 1] * RAD_TO_DEG, window->rasterizer_camera.z_near,
                window->rasterizer_camera.z_far);
    }
    else
    {
        const observer_result *camera = observer_get_camera(idx);
        if ( camera )
        {
            hcex_float3 pos = { camera->position.n[0], camera->position.n[1], camera->position.n[2] };
            hcex_float3 forv = { camera->forward.n[0], camera->forward.n[1], camera->forward.n[2] };
            hcex_float3 up = { camera->up.n[0], camera->up.n[1], camera->up.n[2] };

            hcex_set_cam(idx, &pos, &up, &forv, camera->field_of_view * RAD_TO_DEG, rasterizer_globals.z_near,
                    rasterizer_globals.z_far);
        }
    }
}
