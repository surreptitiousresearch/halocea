#include "../../headers/ws/ai/aiPLATT.h"
#include "../../headers/ws/ai/aiBRAIN.h"        // aiBRAIN::mind
#include "../../headers/ws/ai/aiMIND.h"         // aiMIND::enemy (dsSHARED_PTR)
#include "../../headers/ws/ai/aiENEMY_BASE.h"   // aiENEMY_BASE::IsPlayer

// aiPLATT::ProcessFrame @ 0x83238578
// ?ProcessFrame@aiPLATT@@QAAXXZ
//
// Prune the attacker list back-to-front: any brain whose current enemy is no longer the local
// player is removed. GetPtr() safely resolves each weak handle (locked refcount bump/read/release).
// NOTE: the binary dereferences the resolved brain's enemy module without a null guard — reproduced
// faithfully (a live attacker entry is assumed to still hold a valid brain and enemy module).
void aiPLATT::ProcessFrame()
{
    for (int i = attackers.nElem - 1; i >= 0; --i)
    {
        aiBRAIN *pBrain = attackers[i].GetPtr();
        if (!pBrain->mind.enemy.pointee->IsPlayer())
            attackers.Erase(i, 1);
    }
}
