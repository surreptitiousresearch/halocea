#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;          // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;       // shared assert-info byte @0x8200155A
extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// aiBEST::FindMajorResponsibleForBody @ 0x83297D50
// ?FindMajorResponsibleForBody@aiBEST@@AAAHXZ
//
// Scan the changeable stack from the top down for the first (topmost) non-null behaviour that is NOT
// a sub-behaviour (BHV_ST_SUBBEHAV clear) — i.e. the "major" behaviour currently responsible for the
// body. Returns its stack index, or -1 if none. NULL slots in the middle of the stack are logged and
// skipped.
int aiBEST::FindMajorResponsibleForBody()
{
    for (int idx = arrChangeable.nEntry - 1; idx >= 0; --idx) {
        if (!IGNORE_STRONG_ASSERT && (idx >= 10 || idx >= arrChangeable.nEntry))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, byte_8200155A);

        bhvBEHAV *cur = arrChangeable.list[idx];
        if (!cur) {
            _apLog("~AI,Error~%s: NULL in the middle of the stack.", pBrain->names.nameInst);
            continue;
        }
        if ((cur->state.val & BHV_ST_SUBBEHAV) == 0)
            return idx;
    }
    return -1;
}
