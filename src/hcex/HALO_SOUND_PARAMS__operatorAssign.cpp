#include "../headers/hcex/HALO_SOUND_PARAMS.h"

// 0x836BFC88 — ??4HALO_SOUND_PARAMS@@QAAAAU0@ABU0@@Z (operator=). Copies every scalar/dsSTRID/
// dsFLAGS field, then delegates `replace` to dsVECTOR::operator= (which resizes+copies in place).
HALO_SOUND_PARAMS &HALO_SOUND_PARAMS::operator=(const HALO_SOUND_PARAMS &that)
{
    this->tag.id = that.tag.id;
    this->state.val = that.state.val;
    this->volume = that.volume;
    this->volume2D = that.volume2D;
    this->distGt = that.distGt;
    this->distGtTag.id = that.distGtTag.id;
    this->distLt = that.distLt;
    this->distLtTag.id = that.distLtTag.id;
    this->dist2D = that.dist2D;
    this->dist3D = that.dist3D;
    this->mouthTag.id = that.mouthTag.id;
    this->mouthMinDelay = that.mouthMinDelay;
    this->replace = that.replace;
    return *this;
}
