#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dsc/dscDESC.h"

// aiBRAIN::CopyDataFromDescToStatus @ 0x831789D8
// ?CopyDataFromDescToStatus@aiBRAIN@@IAAXXZ  (protected)
//
// Copy descriptor-driven configuration into the runtime status block: two general-status flags,
// the smart death max-distance (defaulting to -1 when disabled), and the dynamic-nav-sampling
// parameters. All descriptor fields read here live in the derived (brain) descriptor past
// dscDESC's own members — the subclass layout is the next frontier, so read by verified byte offset.
void aiBRAIN::CopyDataFromDescToStatus()
{
    dscDESC      *pDesc = spDesc.pointee;
    aiBRAIN_DESC *brainDesc = reinterpret_cast<aiBRAIN_DESC *>(pDesc); // runtime descriptor is the derived brain descriptor
    aiSTATUS     *pStatus = st.pointee;
    unsigned char *descBytes = reinterpret_cast<unsigned char *>(pDesc);

    if (descBytes[0x1FA]) pStatus->gen.val |= 0x8000;   else pStatus->gen.val &= ~0x8000;
    if (descBytes[0x1FF]) pStatus->gen.val |= 0x200000; else pStatus->gen.val &= ~0x200000;

    if (descBytes[0x2B7])
        pStatus->smtDeathMaxDist = brainDesc->smtDeathMaxDist;
    else
        pStatus->smtDeathMaxDist = -1.0f;

    pStatus->dns.nPoints = brainDesc->dns.nPoints;
    pStatus->dns.distMin = brainDesc->dns.distMin;
    pStatus->dns.distMax = brainDesc->dns.distMax;
}
