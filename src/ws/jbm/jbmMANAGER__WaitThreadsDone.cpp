#include "../../headers/ws/jbm/jbmMANAGER.h"
#include "../ds/ds_assert_boundary.h"

extern "C" int osWaitForSignal(osHANDLE_DUMMY *signal, unsigned long msTimeout); // boundary — os subsystem

// 0x826EBE08 jbmMANAGER::WaitThreadsDone(apSTATE_T<unsigned long>) — block until every live
// worker thread whose index bit is set in threadMask.state has drained its jobDeque
// (osWaitForSignal on threadDone, infinite timeout). Walks thread indices from
// (threads.nEntry-1) down to 0 (disasm rotate recurrence, expressed as `1<<idx` here); each
// in-mask index is bounds-checked with the STRONG_ASSERT IsValidIdx pattern (source:
// ds_const_list.h:166) before the dsCONST_ARRAY access.
void jbmMANAGER::WaitThreadsDone(apSTATE_T<unsigned long> threadMask)
{
    unsigned long mask = threadMask.state;

    for (int threadIdx = threads.nEntry - 1; threadIdx >= 0; --threadIdx)
    {
        if ((1u << threadIdx) & mask)
        {
            if (!IGNORE_STRONG_ASSERT &&
                ((unsigned int)threadIdx >= 0x10 || threadIdx >= threads.nEntry))
            {
                STRONG_ASSERT_DUMMY().Crash(
                    "IsValidIdx(idx)",
                    "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h",
                    166,
                    empty_string);
            }

            if (threads[threadIdx].startThread)
                osWaitForSignal(threads[threadIdx].threadDone, 0xFFFFFFFF);
        }
    }
}
