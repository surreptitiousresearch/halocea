/* ?SetStereo3DRatio@gsRENDER_SYSTEM@@QAAXM@Z @0x823CDD10 */
#include "../../headers/ws/gs/gsRENDER_SYSTEM.h"

// 0x823CDD10 — set the stereo-3D eye-separation ratio. The parameter arrives as a float in the
// FP arg register; the decompiler widens it to double, but the stored field is a 4-byte float.
void gsRENDER_SYSTEM::SetStereo3DRatio(float ratio)
{
    stereo3DRatio = ratio;
}
