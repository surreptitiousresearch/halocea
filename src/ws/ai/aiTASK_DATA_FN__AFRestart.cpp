// aiTASK_DATA_FN::AFRestart @ 0x83260D20
// ?AFRestart@aiTASK_DATA_FN@@IAAXPAVaiBRAIN@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/bhp/bhpSNS_PAR.h"

// Protected: arm the after-fire wait. Unless already waiting, reset the AF timer to the configured
// delay, enter WAIT_AF, and command the brain's best-behaviour module into the "@TaskSys_Wait_AF@"
// stay-&-notify behaviour. The temporary params block self-destructs at scope end (the decompiler
// shows the destructor — vtable restore + freeing the base fail-notify vector — inlined here).
void aiTASK_DATA_FN::AFRestart(aiBRAIN *pBrain)
{
    if (state == AI_TASK_DATA_FN_WAIT_AF)
        return;

    timerAF = afDelay;
    state = AI_TASK_DATA_FN_WAIT_AF;

    bhpSNS_PAR params;
    pBrain->mind.best.pointee->SetBehaviorByParams(&params, "@TaskSys_Wait_AF@");
}
