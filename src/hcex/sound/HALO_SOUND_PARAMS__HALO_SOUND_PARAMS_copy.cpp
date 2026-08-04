// ??0HALO_SOUND_PARAMS@@QAA@ABU0@@Z -- 0x836BFBE8 (copy ctor)
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

HALO_SOUND_PARAMS::HALO_SOUND_PARAMS(const HALO_SOUND_PARAMS &that)
    : tag(that.tag), state(), volume(that.volume), volume2D(that.volume2D),
      distGt(that.distGt), distGtTag(that.distGtTag), distLt(that.distLt),
      distLtTag(that.distLtTag), dist2D(that.dist2D), dist3D(that.dist3D),
      mouthTag(that.mouthTag), mouthMinDelay(that.mouthMinDelay), replace(that.replace.__cl)
{
    state.val = that.state.val;
    replace.Insert(0, that.replace.pData, that.replace.nElem);
}
