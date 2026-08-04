#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"

// aiMIND::ValidateEnemySystem @ 0x8322B798
// ?ValidateEnemySystem@aiMIND@@IAAXXZ  (protected)
//
// Reconcile the enemy module with the tracked enemy's live/dead state: if the enemy entity has gone
// (Is(null)) and is not already frozen, freeze it; otherwise, if it is certainly dead, zero it.
void aiMIND::ValidateEnemySystem()
{
    if (enemy.pointee->IsZero())
        return;

    if (!enemy.pointee->IsFrozen())
    {
        // disasm: vtbl+0x28 Is(entENTITY*) invoked with a null argument (decompiler dropped it).
        if (enemy.pointee->Is(nullptr))
        {
            FreezeEnemy();
            return;
        }
    }

    if (enemy.pointee->IsDeadForSure())
        ZeroEnemy();
}
