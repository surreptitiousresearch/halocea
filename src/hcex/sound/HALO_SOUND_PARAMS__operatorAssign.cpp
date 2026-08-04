// ??4HALO_SOUND_PARAMS@@QAAAAU0@ABU0@@Z -- 0x836BFCAC
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

HALO_SOUND_PARAMS &HALO_SOUND_PARAMS::operator=(const HALO_SOUND_PARAMS &that)
{
    tag = that.tag;
    state.val = that.state.val;
    volume = that.volume;
    volume2D = that.volume2D;
    distGt = that.distGt;
    distGtTag = that.distGtTag;
    distLt = that.distLt;
    distLtTag = that.distLtTag;
    dist2D = that.dist2D;
    dist3D = that.dist3D;
    mouthTag = that.mouthTag;
    mouthMinDelay = that.mouthMinDelay;
    replace = that.replace;
    return *this;
}
