#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;      // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;   // shared empty-string / assert-info byte @0x8200155A
extern int _dbgAICountAllocatedBehaviors; // ?_dbgAICountAllocatedBehaviors@@3HA — live-behaviour count

// aiBEST::TrashPopped @ 0x83297108
// ?TrashPopped@aiBEST@@AAAXXZ
//
// Issue any pending terminate calls, then destroy every behaviour queued in the to-delete stack:
// null it out of the execution stack, hand it to the brain's trash bin for deferred destruction,
// decrement the global allocated-behaviour debug count, and clear the slot. Finally empties the
// to-delete stack.
void aiBEST::TrashPopped()
{
    IssueTermCalls();

    if (arrToDel.nEntry > 0) {
        for (int i = 0; i < arrToDel.nEntry; ++i) {
            bhvBEHAV *b = arrToDel[i];

            for (int j = 0; j < arrExecutable.nEntry; ++j) {
                if (!IGNORE_STRONG_ASSERT && ((unsigned int)j > 9 || j >= arrExecutable.nEntry))
                    STRONG_ASSERT_DUMMY::Crash(nullptr, "IsValidIdx(idx)",
                        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, byte_8200155A);
                if (arrExecutable.list[j] == b)
                    arrExecutable[j] = nullptr;
            }

            pBrain->bin.Trash(b);
            --_dbgAICountAllocatedBehaviors;

            if (!IGNORE_STRONG_ASSERT && ((unsigned int)i >= 0x14 || i >= arrToDel.nEntry))
                STRONG_ASSERT_DUMMY::Crash(nullptr, "IsValidIdx(idx)",
                    "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, byte_8200155A);
            arrToDel.list[i] = nullptr;
        }
    }

    arrToDel.nEntry = 0;
}
