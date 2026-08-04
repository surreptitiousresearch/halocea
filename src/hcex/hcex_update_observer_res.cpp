/* hcex_update_observer_res @0x8369E200 — final fix-up of the engine observer/camera result from the
 * Blam (HCEX) side. When the Halo camera is driving (halo_cam_sync_back) it overrides the result's
 * position/forward/up with the latched Halo camera basis; when widescreen FOV correction is enabled
 * it applies a hor+ aspect adjustment to the field of view.
 *
 * DEVIATION: the decompiler invents a long double `a2` param used only as FP scratch; the real
 * signature takes just the observer_result (matching the existing call-site extern). */

#include "../headers/observer_result.h"
#include "../headers/hcex/hcex_float3.h"

extern int         halo_cam_sync_back;
extern int         halo_adjust_widescreen_fov;
extern hcex_float3 halo_cam_pos;
extern hcex_float3 halo_cam_forv;
extern hcex_float3 halo_cam_up;

extern double tan(double x);
extern double atan2(double y, double x);

extern "C" void hcex_update_observer_res(observer_result *cmd)
{
    if (halo_cam_sync_back)
    {
        cmd->position.n[0] = halo_cam_pos.x;
        cmd->position.n[1] = halo_cam_pos.y;
        cmd->position.n[2] = halo_cam_pos.z;
        cmd->forward.n[0] = halo_cam_forv.x;
        cmd->forward.n[1] = halo_cam_forv.y;
        cmd->forward.n[2] = halo_cam_forv.z;
        cmd->up.n[0] = halo_cam_up.x;
        cmd->up.n[1] = halo_cam_up.y;
        cmd->up.n[2] = halo_cam_up.z;
    }

    if (halo_adjust_widescreen_fov)
    {
        /* hor+ conversion: fov' = 2 * atan(tan(fov/2) * (4/3)). */
        float half_fov = cmd->field_of_view * 0.5f;
        float scaled_tan = (float)tan(half_fov) * 1.3333334f;
        cmd->field_of_view = (float)atan2(scaled_tan, 1.0) * 2.0f;
    }
}
