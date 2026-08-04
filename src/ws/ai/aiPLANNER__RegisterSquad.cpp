#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiSQUAD.h"

// aiPLANNER::RegisterSquad(aiSQUAD*) — 0x831673E0
// Append pSquad to arrSquad if not already present. The extra a3..a14 params the PPC decompiler
// invented are stack scratch (a14 is the by-value copy of pSquad handed to PushBack); the true
// ABI is a single aiSQUAD* argument.
void aiPLANNER::RegisterSquad(aiSQUAD *pSquad)
{
    bool alreadyRegistered = false;
    int  nElem = arrSquad.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (arrSquad[i] == pSquad)
            alreadyRegistered = true;
    }

    if (!alreadyRegistered)
        arrSquad.PushBack(pSquad);
}
