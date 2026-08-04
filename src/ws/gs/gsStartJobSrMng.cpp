// gsStartJobSrMng @0x82515BC8 — gs_msg.cpp
// Dispatch one spatial-refine deferred job. The destination depends on the active multi-thread
// message configuration (gsMsgMTConfig):
//   config 2 -> hand the job to the job manager on thread mask 4
//   config 4 -> hand the job to the job manager on thread mask 32 (0x20)
//   otherwise (single-thread) -> queue it on the manager's own srJobs list for inline execution.
#include "../../headers/ws/gs/gsSR_MANAGER.h"
#include "../../headers/ws/jbm/jbmMANAGER.h"
#include "../../headers/ws/ap/apSTATE_T.h"

extern unsigned int gsMsgMTConfig;   // boundary — active MT message configuration
extern jbmMANAGER   gsJobManager;    // boundary — global frame-job manager

void gsStartJobSrMng(jbmJOB *pJob)
{
    if (gsMsgMTConfig == 2)
    {
        gsJobManager.AddJob(apSTATE_T<unsigned long>{4}, pJob);
    }
    else if (gsMsgMTConfig == 4)
    {
        gsJobManager.AddJob(apSTATE_T<unsigned long>{0x20}, pJob);
    }
    else
    {
        pSrMng->srJobs.PushBack(pJob);
    }
}
