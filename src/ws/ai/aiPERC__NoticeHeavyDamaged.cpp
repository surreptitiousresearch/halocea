#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ent/entENTITY.h"

// aiPERC::NoticeHeavyDamaged @ 0x8329D5D0
// ?NoticeHeavyDamaged@aiPERC@@UAAXPAVentENTITY@@@Z
//
// Heavy damage from `shooter` forces it to become the tracked enemy, locks the enemy from changing
// for the descriptor's cell-51 duration, and disables further injury reaction for cell-52 seconds.
void aiPERC::NoticeHeavyDamaged(entENTITY *shooter)
{
    if (!shooter)
        return;

    pBrain->mind.AssignEnemy(shooter, true);
    aiBRAIN *b = pBrain;
    dscDESC *desc = b->spDesc.pointee;
    b->mind.enemy.pointee->LockChange(*(float *)&desc[51].pBrand);
    injury.timerDisable = *(float *)&desc[52].__vftable;
}
