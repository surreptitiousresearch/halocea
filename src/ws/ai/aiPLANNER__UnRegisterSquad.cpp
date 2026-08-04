#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiSQUAD.h"

// aiPLANNER::UnRegisterSquad(aiSQUAD*) — 0x83166E38
// Remove pSquad from arrSquad via swap-with-last-then-erase (order not preserved).
void aiPLANNER::UnRegisterSquad(aiSQUAD *pSquad)
{
    int nElem = arrSquad.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (arrSquad[i] != pSquad)
            continue;

        int lastIdx = arrSquad.nElem - 1;
        if (i < lastIdx)
            arrSquad[i] = arrSquad[lastIdx];
        arrSquad.Erase(lastIdx, 1);
        return;
    }
}
