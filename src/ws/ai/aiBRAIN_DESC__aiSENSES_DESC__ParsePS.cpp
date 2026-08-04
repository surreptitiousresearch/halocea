#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiBRAIN_DESC::aiSENSES_DESC::ParsePS @ 0x83185B90
// ?ParsePS@aiSENSES_DESC@aiBRAIN_DESC@@QAAXABVpsSECTION@@@Z
//
// Read the sight-cone half-angles and hearing radii from `ps` (leaving current values as defaults
// when a key is absent), then enforce side >= straight for both the see-angle and the see-radius.
// The by-value GetFloat overload takes (key, default, flags); the decompiler's phantom trailing
// arg is the flags slot — always 0 here.
void aiBRAIN_DESC::aiSENSES_DESC::ParsePS(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);
    if (ps.IsEmpty())
        return;

    this->angleSeeStraight = ps.GetFloat("angleSeeStraight", this->angleSeeStraight, 0);
    this->angleSeeSide     = ps.GetFloat("angleSeeSide", this->angleSeeSide, 0);
    this->radSeeStraight   = ps.GetFloat("radSeeStraight", this->radSeeStraight, 0);
    this->radSeeSide       = ps.GetFloat("radSeeSide", this->radSeeSide, 0);
    this->radHearShot      = ps.GetFloat("radHearShot", this->radHearShot, 0);
    this->radHearRun       = ps.GetFloat("radHearRun", this->radHearRun, 0);
    this->radHearWalk      = ps.GetFloat("radHearWalk", this->radHearWalk, 0);

    if (this->angleSeeSide < this->angleSeeStraight)
        this->angleSeeSide = this->angleSeeStraight;
    if (this->radSeeStraight < this->radSeeSide)
        this->radSeeStraight = this->radSeeSide;
}
