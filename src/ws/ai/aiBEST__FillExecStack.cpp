#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;      // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;   // shared empty-string / assert-info byte @0x8200155A
extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// aiBEST::FillExecStack @ 0x83297270
// ?FillExecStack@aiBEST@@AAAXXZ
//
// Rebuild the flattened execution stack from the changeable stack: clear all ten slots, then append
// each changeable behaviour that is not already present (a duplicate is a logged error and skipped).
void aiBEST::FillExecStack()
{
    arrExecutable.nEntry = 10;
    for (int i = 0; i < 10; ++i)
        arrExecutable[i] = nullptr;

    int nEntry = arrChangeable.nEntry;
    arrExecutable.nEntry = 0;
    if (nEntry <= 0)
        return;

    for (int i = 0; i < arrChangeable.nEntry; ++i) {
        if (!IGNORE_STRONG_ASSERT && ((unsigned int)i > 9 || i >= arrChangeable.nEntry))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, byte_8200155A);

        bhvBEHAV *b = arrChangeable.list[i];

        bool dup = false;
        for (int k = 0; k < arrExecutable.nEntry; ++k)
            if (arrExecutable.list[k] == b) { dup = true; break; }

        if (!dup) {
            if (!IGNORE_STRONG_ASSERT && arrExecutable.nEntry >= 10)
                STRONG_ASSERT_DUMMY::Crash(nullptr, "Length() < size",
                    "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 175, byte_8200155A);
            arrExecutable.list[arrExecutable.nEntry++] = b;
        } else {
            _apLog("~AI,Error~%s: found double behavior '%s'", pBrain->names.nameInst, b->SLID);
        }
    }
}
