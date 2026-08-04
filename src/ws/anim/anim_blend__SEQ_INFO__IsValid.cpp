#include "anim_blend_SEQ_INFO.h"
#include "animSEQ.h"

// anim_blend::SEQ_INFO::IsValid @ 0x82BF8CD8 (const)
// A blend-layer sequence slot is valid when it resolves to a sequence, carries a real
// sequence id, and that sequence is loaded (animSEQ::stateSeq & 0x100).
int anim_blend::SEQ_INFO::IsValid() const
{
    animSEQ *pSeq = this->pSeq;
    if (!pSeq)
        return 0;
    if (this->seqID.seqID == -1)
        return 0;
    if ((pSeq->stateSeq & 0x100) == 0)
        return 0;
    return 1;
}
