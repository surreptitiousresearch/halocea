#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A

// aiBEST::EndBodyResponsibleMajor @ 0x83297EE8
// ?EndBodyResponsibleMajor@aiBEST@@AAAXXZ
//
// Walk the changeable stack from the top down, issuing an "end body" command to each activated
// (BHV_ST_ACTIVATED) behaviour, stopping at the first behaviour that is not a sub-behaviour
// (BHV_ST_SUBBEHAV clear) — i.e. once the body-responsible major has been ended.
void aiBEST::EndBodyResponsibleMajor()
{
    int idx = arrChangeable.nEntry - 1;
    if (idx < 0)
        return;

    do {
        if (!IGNORE_STRONG_ASSERT && (idx >= 10 || idx >= arrChangeable.nEntry))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, byte_8200155A);

        bhvBEHAV *cur = arrChangeable.list[idx];
        if (cur && (cur->state.val & BHV_ST_ACTIVATED) != 0) {
            cur->IssueBodyCmd_ToEnd();
            if ((cur->state.val & BHV_ST_SUBBEHAV) == 0)
                break;
        }
        --idx;
    } while (idx >= 0);
}
