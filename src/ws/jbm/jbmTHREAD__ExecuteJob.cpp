#include "../../headers/ws/jbm/jbmTHREAD.h"

// 0x826EBA90 jbmTHREAD::ExecuteJob(jbmJOB*) — run one job to completion on this worker
// thread, driving its three vtable lifecycle callbacks in order (PreExecute, Execute,
// PostExecute — verified against disasm: vtbl slot loads at +8, +4, +0xC respectively,
// matching the DB jbmJOB_vtbl layout).
void jbmTHREAD::ExecuteJob(jbmJOB *pJob)
{
    pJob->PreExecute(procID);
    pJob->Execute(procID);
    pJob->PostExecute(procID);
}
