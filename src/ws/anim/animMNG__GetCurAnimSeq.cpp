#include "animMNG.h"
#include "animINST.h"
#include "animLAYER_BASE.h"
#include "anim_blend_SEQ_INFO.h"

// ?GetCurAnimSeq@animMNG@@QAA?AUSEQ_ID_FULL@anim_blend@@H@Z  (0x8267C430) — the sequence currently
// playing on blend layer `layerId`, returned by value (sret). The return slot is r3, `this` is r4,
// `layerId` is r5 (the decompiler swapped `this` and the return buffer). Yields {-1,-1} when the
// layer has no valid sequence.
anim_blend::SEQ_ID_FULL animMNG::GetCurAnimSeq(int layerId)
{
    anim_blend::SEQ_ID_FULL result;
    animLAYER_BASE *pLayer;
    anim_blend::SEQ_INFO *pSeqInfo;

    if (this->layerList.nElem
        && (pLayer = this->layerList[layerId].val.pointee) != nullptr
        && (pSeqInfo = pLayer->GetCurAnimSeq()) != nullptr
        && pSeqInfo->IsValid())
    {
        result.seqID = pSeqInfo->seqID.seqID;
        result.extID = pSeqInfo->seqID.extID; // disasm copies the full 4-byte extID word
    }
    else
    {
        result.seqID = -1;
        result.extID = -1;
    }
    return result;
}
