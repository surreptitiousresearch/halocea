/* ?ResetLightsMaskIfNeeded@rendDYN_LIGHT_STATE@@IAAXI@Z @0x8273E290 */
#include "../../headers/ws/rend/rendDYN_LIGHT_STATE.h"

// 0x8273E290 — lastLgtUpdFrameNmb packs a 30-bit frame number in bits [0,29] plus a sticky
// "force reset" flag in bit 30 (0x40000000). If the flag is set, or the stored frame number
// already matches the current one (both masked to 30 bits), the cached masks are still valid
// and nothing happens. Otherwise the per-side dynamic/shadow-map light masks and counts are
// cleared and the frame number is updated (the sticky flag is always cleared on update, since
// only the low 30 bits of frameNmb are stored back).
void rendDYN_LIGHT_STATE::ResetLightsMaskIfNeeded(unsigned int frameNmb)
{
    const unsigned int RESET_FORCED_FLAG = 0x40000000;
    const unsigned int FRAME_NUMBER_MASK = 0x3FFFFFFF;

    if ((lastLgtUpdFrameNmb & RESET_FORCED_FLAG) == 0 &&
        (lastLgtUpdFrameNmb & FRAME_NUMBER_MASK) != (frameNmb & FRAME_NUMBER_MASK))
    {
        for (int side = 0; side < 2; ++side)
        {
            dynLightsMask[side] = 0;
            dynLightsMaskFiltered[side] = 0;
            nDynLights[side] = 0;
            nSMLights[side] = 0;
        }
        lastLgtUpdFrameNmb = frameNmb & FRAME_NUMBER_MASK;
    }
}
