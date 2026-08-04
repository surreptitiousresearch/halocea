// aiMIND::ValidateSeleneSystem @ 0x8322C110
// ?ValidateSeleneSystem@aiMIND@@IAAXXZ  (protected)
//
// If the tracked enemy went null while the mind is still in scripted-enemy mode (est bit 0x80000),
// tear the scripted state down: zero the enemy, drop the scripted bit, reset sparsings, and restore
// the normal selene.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"

void aiMIND::ValidateSeleneSystem()
{
    aiBRAIN  *brain = static_cast<aiBRAIN *>(pBrain);
    aiSTATUS *st    = brain->st.pointee;

    if (enemy.pointee->IsZero() && ((st->est.val >> 19) & 1) != 0)
    {
        ZeroEnemy();
        brain->st.pointee->est.val &= ~0x80000;
        brain->ResetAllSparsings();
        SetNormalSelene();
    }
}
