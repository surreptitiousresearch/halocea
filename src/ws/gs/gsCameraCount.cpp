// gsCameraCount @ 0x82703C10 — number of registered render cameras.
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/cam/camCAMERA.h"

extern dsVECTOR<camCAMERA *, 8> vec_cam; // global camera registry

int gsCameraCount()
{
    return vec_cam.nElem;
}
