// gsCameraGet @ 0x827044F0 — fetch the render camera at `idx`, or null if out of range.
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/cam/camCAMERA.h"

extern dsVECTOR<camCAMERA *, 8> vec_cam; // global camera registry

camCAMERA *gsCameraGet(int idx)
{
    if (idx < 0 || idx >= vec_cam.nElem)
        return nullptr;
    return vec_cam[idx];
}
