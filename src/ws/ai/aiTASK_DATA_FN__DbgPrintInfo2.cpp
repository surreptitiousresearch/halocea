// aiTASK_DATA_FN::DbgPrintInfo2 @ 0x83261430
// ?DbgPrintInfo2@aiTASK_DATA_FN@@MAAXAAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern dsTSTRING<char> dsSPrintf(const char *fmt, ...); // ds — format into a fresh dsTSTRING  boundary

// Protected virtual: append the run-state label to `out`. Any out-of-range state prints nothing.
void aiTASK_DATA_FN::DbgPrintInfo2(dsTSTRING<char> *out)
{
    if ((unsigned int)state > AI_TASK_DATA_FN_DONE)
        return;

    const char *label;
    switch (state) {
    case AI_TASK_DATA_FN_WAIT_REPLY:
        label = "Wait Reply";
        break;
    case AI_TASK_DATA_FN_WAIT_AF: {
        dsTSTRING<char> text = dsSPrintf("Wait AF(%.1f)", timerAF);
        *out += text;
        return;
    }
    case AI_TASK_DATA_FN_READY_AF:
        label = "Ready AF";
        break;
    case AI_TASK_DATA_FN_DONE:
        label = "Done";
        break;
    default: // AI_TASK_DATA_FN_READY
        label = "Ready";
        break;
    }
    out->Insert(out->pBuffer->strLen, label, -1);
}
