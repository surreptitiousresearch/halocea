#include "../../headers/ws/ai/aiBEST.h"

// aiBEST::BeginBodyResponsibleMajor @ 0x83297E30
// ?BeginBodyResponsibleMajor@aiBEST@@AAAXH@Z
//
// After a stack edit, re-issue "begin body" commands. `oldReMajor` is the index of the major that
// was responsible for the body before the edit. If the major responsible for the body has changed
// (and both indices are valid), every behaviour from the new major up to the top that is activated
// (BHV_ST_ACTIVATED) but not finished (BHV_ST_FINISHED) is told to begin its body.
void aiBEST::BeginBodyResponsibleMajor(int oldReMajor)
{
    if (oldReMajor < 0)
        return;

    int newReMajor = FindMajorResponsibleForBody();
    if (newReMajor < 0 || oldReMajor == newReMajor)
        return;

    for (int idx = newReMajor; idx < arrChangeable.nEntry; ++idx) {
        bhvBEHAV *cur = arrChangeable[idx];
        if (cur) {
            unsigned int st = cur->state.val;
            if ((st & BHV_ST_ACTIVATED) != 0 && (st & BHV_ST_FINISHED) == 0)
                cur->IssueBodyCmd_ToBegin();
        }
    }
}
