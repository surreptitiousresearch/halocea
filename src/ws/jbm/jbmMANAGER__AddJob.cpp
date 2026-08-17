/* ?AddJob@jbmMANAGER@@QAAXV?$apSTATE_T@K@@PAVjbmJOB@@@Z @0x826ECAE0 */
#include "../../headers/ws/jbm/jbmMANAGER.h"
#include "../../headers/ws/jbm/jbmJOB.h"

extern "C" int osGetCurThreadProcessor(); // boundary — os subsystem

// 0x826ECAE0 jbmMANAGER::AddJob(apSTATE_T<unsigned long>, jbmJOB*) — route pJob to every
// live worker thread whose index bit is set in threadMask.state. Bit 28 is a "current
// processor" wildcard: when set, OR in the bit for the calling thread's own processor
// before scanning. Walks thread indices from (threads.nEntry-1) down to 0 (disasm uses a
// rotate-based `1<<idx` recurrence; expressed here as the equivalent shift per iteration).
void jbmMANAGER::AddJob(apSTATE_T<unsigned long> threadMask, jbmJOB *pJob)
{
    unsigned long mask = threadMask.state;

    if ((mask >> 28) & 1)
        mask |= (1u << osGetCurThreadProcessor());

    for (int threadIdx = threads.nEntry - 1; threadIdx >= 0; --threadIdx)
    {
        if ((1u << threadIdx) & mask)
        {
            if (threads[threadIdx].startThread)
                threads[threadIdx].AddJob(pJob);
        }
    }
}
