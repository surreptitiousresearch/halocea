#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// dsSPrintf @ 0x825210C0 (?dsSPrintf@@YA?AV?$dsTSTRING@D@@PBDZZ) — formatted build into a new
// dsTSTRING (returned by value). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);

// aiBRAIN_DESC::ReadNav @ 0x83186310
// ?ReadNav@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// Read the navigation tuning: the per-peer edge-weight table ("navEdgePeerWeights" sub-section,
// keyed "weightPeer_00".."weightPeer_NN"), the hard-obstacle enter shift/penalty scalars, and the
// nav-association distance.
void aiBRAIN_DESC::ReadNav(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);

    if (ps.IsKeyExist("navEdgePeerWeights", 0))
    {
        psSECTION weights = ps.GetSection("navEdgePeerWeights", 0);
        const psSECTION_INTERFACE &wi = static_cast<const psSECTION_INTERFACE &>(weights);

        int count = this->navEdgePeerWeight.nEntry;
        for (int i = 0; i < count; ++i)
        {
            dsTSTRING<char> key = dsSPrintf("weightPeer_%.2d", i);
            float &slot = this->navEdgePeerWeight[i];
            slot = wi.GetFloat(key.CStr(), slot, 0);
        }
    }

    this->enterHOPenaltyCenterShiftToHO =
        ps.GetFloat("enterHOPenaltyCenterShiftToHO", this->enterHOPenaltyCenterShiftToHO, 0);
    this->enterHOPenaltyCenterShiftToCorner =
        ps.GetFloat("enterHOPenaltyCenterShiftToCorner", this->enterHOPenaltyCenterShiftToCorner, 0);
    this->enterHOPosShiftToHO =
        ps.GetFloat("enterHOPosShiftToHO", this->enterHOPosShiftToHO, 0);
    this->enterHOPosShiftToCorner =
        ps.GetFloat("enterHOPosShiftToCorner", this->enterHOPosShiftToCorner, 0);
    this->navAssocDistance = ps.GetFloat("navAssocDistance", this->navAssocDistance, 0);
}
