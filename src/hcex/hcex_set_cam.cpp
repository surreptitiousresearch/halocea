/* hcex_set_cam @0x823D2ED8 — HCEX bridge: drive the ws-engine camera for render window `idx` from the Blam
 * side, or, when the "off cam" latch is set, read the ws camera transform back into the halo_cam_* globals so
 * the Blam observer can be overridden (see hcex_update_observer_res). A pending dump-models debug request is
 * serviced first. No-op once a cinematic (Anitec) camera is driving.
 *
 * The pos/up/forv inputs are in the halo axis convention; both directions convert to ws as
 * (x, y, z) -> (x, z, -y). The position is scaled by 3.0480001 (halo world unit -> ws), offset into world
 * space (hcexWorldOffset) and pushed forward along the view by offsetZ before being handed to the camera.
 * The horizontal FOV honours the dbg_hcex_CustomFov override when it exceeds 10 degrees.
 *
 * DEVIATION: the decompiler invented a phantom uninitialised m3dV* as _m3dLineCombine's output argument;
 * the disassembly (r6 == &position) shows the output IS the position vector itself, i.e. the combine is
 * done in place (position += offsetZ * forward). Reconstructed accordingly. */

#include "../headers/hcex/hcex_set_cam_boundary.h"

extern "C" void hcex_set_cam(int idx, hcex_float3 *pos, hcex_float3 *up, hcex_float3 *forv,
        float fov, float z_near, float z_far)
{
    if ( dbg_hcex_DumpModels.base.value || dumpModels )
    {
        dumpModels = 0;
        hcex_dump_all_models();
    }

    if ( idx >= gsCameraCount() )
        return;

    camCAMERA *camera = gsCameraGet(idx);
    if ( !camera )
        return;

    halo_cam_sync_back = 0;

    if ( hcex_off_cam )
    {
        if ( !idx )
        {
            m3dV camera_pos;
            camera_pos.x = camera->matrC2W.elements[12];
            camera_pos.y = camera->matrC2W.elements[13];
            camera_pos.z = camera->matrC2W.elements[14];
            _m3dCheckValid(&camera_pos);

            /* ws->halo: scale by 1/3.048 and axis-swap (x, y, z) -> (x, -z, y). */
            halo_cam_pos.x = camera_pos.x * WS_TO_HALO_SCALE;
            halo_cam_pos.y = camera_pos.z * -WS_TO_HALO_SCALE;
            halo_cam_pos.z = camera_pos.y * WS_TO_HALO_SCALE;

            halo_cam_up.x = camera->matrC2W.elements[4];
            halo_cam_up.y = -camera->matrC2W.elements[6];
            halo_cam_up.z = camera->matrC2W.elements[5];
            halo_cam_forv.x = camera->matrC2W.elements[8];
            halo_cam_forv.y = -camera->matrC2W.elements[10];
            halo_cam_forv.z = camera->matrC2W.elements[9];
        }
        halo_cam_sync_back = 1;
    }
    else if ( !gsSysAnitec->pObjCam )
    {
        m3dV position;
        m3dV forward;
        m3dV up_axis;
        m3dV right;

        /* halo->ws axis swap: (x, y, z) -> (x, z, -y); position also scaled to ws units. */
        position.x = pos->x * HCEX_WORLD_SCALE;
        position.y = pos->z * HCEX_WORLD_SCALE;
        position.z = pos->y * -HCEX_WORLD_SCALE;

        forward.x = forv->x;
        forward.y = forv->z;
        forward.z = -forv->y;

        up_axis.x = up->x;
        up_axis.y = up->z;
        up_axis.z = -up->y;

        m3dCrossProduct(&forward, &up_axis, &right);
        m3dV_add_assign(&position, &hcexWorldOffset);
        _m3dLineCombine(&position, &forward, offsetZ, &position);

        camCAMERA_SetParameters(camera, &position, &up_axis, &right, &forward);
        hcex_cine_set_cam(camera);

        float angle_hor = dbg_hcex_CustomFov.value;
        if ( dbg_hcex_CustomFov.value <= 10.0f )
            angle_hor = fov;
        camCAMERA_SetAngleHor(camera, angle_hor);
    }
}
