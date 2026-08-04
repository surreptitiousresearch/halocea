// aiTASK_DATA_FN::Execute(aiBRAIN*, aiTASK_BASE*) @ 0x83263CE0
// ?Execute@aiTASK_DATA_FN@@UAA_NPAVaiBRAIN@@PAVaiTASK_BASE@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"

// Virtual per-frame step: lazily resolve the scripted function, then dispatch on run state.
bool aiTASK_DATA_FN::Execute(aiBRAIN *pBrain, aiTASK_BASE *taskSys)
{
    if (!this->CreateFnPtrByName())
        return false;

    aiTASK_DATA_FN_STATE state = this->state;
    if ((unsigned int)state > AI_TASK_DATA_FN_DONE)
        return false;
    if (state == AI_TASK_DATA_FN_WAIT_REPLY)
        return true;

    if (state != AI_TASK_DATA_FN_WAIT_AF)
    {
        if (state == AI_TASK_DATA_FN_READY_AF || state == AI_TASK_DATA_FN_READY)
            this->CallFnPtr(pBrain, taskSys);
        return true;
    }

    // WAIT_AF — once the after-fire timer elapses, become ready for the after-fire pass.
    if (this->timerAF < 0.000001f)
    {
        this->state = AI_TASK_DATA_FN_READY_AF;
        this->chanceToCreateBhv = 1;
    }
    return true;
}
