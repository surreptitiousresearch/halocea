/* hcex_scenery_output @0x823D0F68 — HCEX debug helper that appends one placed object's record to the
 * "scenery_obj_list.txt" dump log: its id/type, sanitised model name, halo->ws converted position, and euler
 * orientation. Called repeatedly by hcex_dump_scenery_by_type; a final call with model_name == NULL flushes
 * the log (writes nothing but a right-now-mode toggle). The m3d math and apLog dump systems are ws-engine
 * boundaries.
 *
 * The forward/up basis vectors are converted from the halo axis convention to ws ((x, y, z) -> (x, z, -y)),
 * normalised, and completed with their cross product before building a local->world matrix at the origin and
 * extracting euler angles from it. The position is scaled by 3.0480001 and axis-swapped the same way. */

#include "../headers/hcex/hcex_scenery_output_boundary.h"

extern "C" void hcex_scenery_output(int obj_id, int obj_type, const char *model_name,
        hcex_float3 *pos, hcex_float3 *fw, hcex_float3 *up)
{
    if ( model_name )
    {
        char sanitised_name[64];
        m3dV forward_axis;
        m3dV up_axis;
        m3dV right_axis[2];
        m3dMATR orientation;
        float euler0;
        float euler1;
        float euler2;

        apLogSetDumpRightNowMode(1);
        _apDumpLog("scenery_obj_list.txt", "%04x_%x\n", obj_id, obj_type);
        hcex_conv_mdl_name(model_name, sanitised_name, 64);
        _apDumpLog("scenery_obj_list.txt", "%s\n", sanitised_name);

        /* halo->ws axis swap: (x, y, z) -> (x, z, -y). */
        forward_axis.x = fw->x;
        forward_axis.y = fw->z;
        forward_axis.z = -fw->y;
        up_axis.x = up->x;
        up_axis.y = up->z;
        up_axis.z = -up->y;

        float pos_x = (float)(pos->x * HALO_TO_WS_SCALE);
        float pos_z = (float)(pos->z * HALO_TO_WS_SCALE);
        float pos_y = (float)(pos->y * -HALO_TO_WS_SCALE);

        m3dNormalize(&forward_axis);
        m3dNormalize(&up_axis);
        m3dCrossProduct(&forward_axis, &up_axis, right_axis);
        m3dMATR_MakeLCS2WCS(&orientation, &m3dVZero, &forward_axis, &up_axis, right_axis);
        m3dMATR_GetRotateRot(&orientation, &euler0, &euler1, &euler2);

        _apDumpLog("scenery_obj_list.txt", "%f\n", pos_x);
        _apDumpLog("scenery_obj_list.txt", "%f\n", pos_z);
        _apDumpLog("scenery_obj_list.txt", "%f\n", pos_y);
        _apDumpLog("scenery_obj_list.txt", "%f\n", euler0);
        _apDumpLog("scenery_obj_list.txt", "%f\n", euler1);
        _apDumpLog("scenery_obj_list.txt", "%f\n", euler2);
    }
    else
    {
        apLogSetDumpRightNowMode(1);
        _apDumpLog("scenery_obj_list.txt", (const char *)0);
    }
    apLogSetDumpRightNowMode(0);
}
