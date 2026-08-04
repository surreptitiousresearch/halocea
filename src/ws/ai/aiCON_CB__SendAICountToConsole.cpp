#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/fio/fioFILE.h"

// aiCON_CB::SendAICountToConsole @ 0x83181908
// Recompute the AI unit tallies; when any changed (or forced) push a "#idx: N live/ M dead"
// summary line (plus a "**** LEAKMEM n ****" warning when the planner reports leaked balance
// behaviours) to the AI console page.
bool aiCON_CB::SendAICountToConsole(bool force)
{
    if (!this->IsConnected())
        return false;

    int live = 0, deadUnits = 0, idxLive = -1, idxDead = -1, extra = 0;
    this->GetAICount(&live, &deadUnits, &idxLive, &idxDead, &extra);

    if (live != this->coLiveUnits || deadUnits != this->coDeadUnits ||
        idxLive != this->idxLiveWat || idxDead != this->idxDeadWat || force)
    {
        this->coLiveUnits = live;
        this->coDeadUnits = deadUnits;
        this->idxLiveWat  = idxLive;
        this->idxDeadWat  = idxDead;

        // vararg order verified against disasm @ 0x831819C8..0x831819E4: (idxLive, live, dead)
        dsTSTRING<char> text = dsSPrintf("#%d: %d live/ %d dead", idxLive, live, deadUnits);

        if (aiPlanner)
        {
            int leaked = aiPLANNER_GetLeakBalanceBehaviors(aiPlanner);
            if (leaked)
                text = dsSPrintf("**** LEAKMEM %d ****", leaked);
        }

        fioFILE *msg = &apConsole->StartMsg(this->pageName.pBuffer->str,
                                            this->msgCtrlName.pBuffer->str,
                                            sn_WatcheeCount_1.pBuffer->str);
        int len = text.pBuffer->strLen + 1;
        msg->WriteData(&len, 4, 4);
        msg->WriteData(text.pBuffer->str, text.pBuffer->strLen + 1, 1);

        if (this->pageVersionValid)
            apConsole->SendMsg(*msg);
        else
            apConsole->CancelMsg(*msg);
    }
    return true;
}
