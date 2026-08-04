// cdtINFO::GetVClos @0x82C16098 — accessor returning the closest hit point stored in the record.
#include "../../headers/ws/cdt/cdtINFO.h"

const m3dV *cdtINFO::GetVClos() const
{
    return &vClosest;
}
