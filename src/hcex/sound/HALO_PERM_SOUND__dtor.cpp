// ??1HALO_PERM_SOUND@@QAA@XZ -- 0x836B4FDC
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

HALO_PERM_SOUND::~HALO_PERM_SOUND()
{
    if (!IGNORE_STRONG_ASSERT && (prevFadeOut || fadeIn))
        STRONG_ASSERT_DUMMY().Crash(
            "prevFadeOut == 0 && fadeIn == 0",
            "d:\\projects\\code\\hcex\\sources\\sound\\fmod\\sound_dsound_fmod.h",
            260,
            empty_string);
}
