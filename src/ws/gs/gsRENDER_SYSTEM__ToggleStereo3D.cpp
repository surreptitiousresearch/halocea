/* ?ToggleStereo3D@gsRENDER_SYSTEM@@QAAXXZ @0x827A1D20 */
#include "../../headers/ws/gs/gsRENDER_SYSTEM.h"

// 0x827A1D20 — toggle the stereo-3D enable flag and request a visibility re-evaluation.
// The disassembly implements the boolean negation with cntlzw (count-leading-zeros): for a
// 0/1 input, (_cntlzw(x) & 0x20) != 0 is exactly !x.
void gsRENDER_SYSTEM::ToggleStereo3D()
{
    isNeedUpdateVis = true;
    isStereo3D = !isStereo3D;
}
