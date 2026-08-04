// ??0HALO_SOUND_PARAMS@@QAA@XZ -- 0x836B6EA4 (default ctor)
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

HALO_SOUND_PARAMS::HALO_SOUND_PARAMS()
    : tag(), state(), volume(1.0f), volume2D(1.0f), distGt(0.0f), distGtTag(),
      distLt(0.0f), distLtTag(), dist2D(0.0f), dist3D(0.0f), mouthTag(),
      mouthMinDelay(0), replace(apCL{"D:\\Projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 24})
{
    state.val = 0;
}
