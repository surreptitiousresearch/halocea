// aiMIND::StopScriptedEnemy @ 0x8322BF60
// ?StopScriptedEnemy@aiMIND@@QAAX_N@Z
//
// Leave scripted-enemy mode: optionally clear the tracked enemy, drop the scripted-enemy status bit
// (est 0x80000), reset all sparsings on the brain, and restore the normal selene module.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

void aiMIND::StopScriptedEnemy(bool isClear)
{
    if (isClear)
        ZeroEnemy();

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
    brain->st.pointee->est.val &= ~0x80000;
    brain->ResetAllSparsings();
    SetNormalSelene();
}
