/* ?RecreateSplitScreen@txmTEXTURE@@IAAXXZ @0x826F61C8 */
// ?RecreateSplitScreen@txmTEXTURE@@IAAXXZ  (0x826F61E4)
// Rebuild the two split-screen sub-textures of a split-screen master after a resize: only runs
// when this texture is flagged as a split-screen master (state bit 0x8) and the active video
// mode is actually split (vmState & 0xC00). Each sibling is re-pointed at its sub-rectangle of
// the master's pixel buffer and Resize()d to the per-viewport dimensions.
// Source: D:\Projects\code\common\src.sys ... (txm subsystem)

#include <stdint.h>
#include "../../headers/ws/txm/txmTEXTURE.h"
#include "../../headers/ws/txm/txm_vid_boundary.h" /* vidDriver->pMode->vmState */

// vidMODE::vmState split bits.
enum {
    VMSTATE_SPLIT_ANY        = 0xC00, // either split active
    VMSTATE_SPLIT_HORIZONTAL = 0x800, // 2 columns -> halve width
    VMSTATE_SPLIT_VERTICAL   = 0x400, // 2 rows    -> halve height
};

// txmTEXTURE::state bits touched here.
enum {
    TXM_STATE_SPLITSCREEN_MASTER = 0x8,     // this texture owns split-screen siblings
    TXM_STATE_SPLIT_CHILD_DATA   = 0x80000, // sibling shares the master's pixel buffer
};

void txmTEXTURE::RecreateSplitScreen()
{
    if (!(this->state.state & TXM_STATE_SPLITSCREEN_MASTER))
        return;

    int64_t vmState = vidDriver->pMode->vmState;
    if (!(vmState & VMSTATE_SPLIT_ANY))
        return;

    int divX = (vmState & VMSTATE_SPLIT_HORIZONTAL) ? 2 : 1;
    unsigned int splitW = (unsigned int)(this->sizeX / divX);

    int divY = (vmState & VMSTATE_SPLIT_VERTICAL) ? 2 : 1;
    unsigned int splitH = (unsigned int)(this->sizeY / divY);

    // Only a vertical split gives the sub-textures a non-zero row pitch (they share full rows of
    // the master buffer); a horizontal split leaves pitch at 0.
    unsigned short splitPitch = (vmState & VMSTATE_SPLIT_VERTICAL) ? this->pitchX : 0;

    for (int idx = 0; idx < 2; ++idx) {
        txmTEXTURE *child = this->pSplitScreenTex[idx];
        child->pData       = this->GetSplitData(idx);
        child->pitchX      = splitPitch;
        child->state.state |= TXM_STATE_SPLIT_CHILD_DATA;
        child->Resize(splitW, splitH);
    }
}
