#include "../../headers/ws/ai/aiBEST.h"

// aiBEST::EraseAll @ 0x832980E0
// ?EraseAll@aiBEST@@AAAXW4BHV_ST@@@Z
//
// Tear the entire changeable stack down. `flags` (a BHV_ST value, e.g. BHV_ST_MANUALLY_CANCELLED) is
// OR'd into each behaviour's state as the removal reason. Disables SetBehavior for the duration
// (BEST_ST_SETBHV_DISABLED), ends the body-responsible major, moves every slot to the to-delete list
// (clearing the slot), empties the stack, runs the terminate sequence, then re-enables SetBehavior.
void aiBEST::EraseAll(BHV_ST flags)
{
    if (arrChangeable.nEntry == 0)
        return;

    state.val |= BEST_ST_SETBHV_DISABLED;
    EndBodyResponsibleMajor();

    for (int idx = arrChangeable.nEntry - 1; idx >= 0; --idx) {
        arrChangeable[idx]->state.val |= flags;
        AddToDel(&arrChangeable[idx]);
        arrChangeable[idx] = nullptr;
    }

    arrChangeable.nEntry = 0;
    IssueTermCalls();
    state.val &= ~BEST_ST_SETBHV_DISABLED;
}
