// cdtINFO::GetFaceNormal @0x82744288 — thin forwarder that requests the face normal without
// also fetching the containing polygon (passes a null m3dPOLY* to GetFacePolyNormal).
#include "../../headers/ws/cdt/cdtINFO.h"

int cdtINFO::GetFaceNormal(m3dV *norm) const
{
    return GetFacePolyNormal(nullptr, norm);
}
