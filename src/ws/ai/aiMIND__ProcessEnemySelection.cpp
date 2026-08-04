// aiMIND::ProcessEnemySelection @ 0x8322C1A8
// ?ProcessEnemySelection@aiMIND@@IAAXXZ  (protected)
//
// Under the "b-selene" profiling counter: re-validate the enemy and selene systems, then run one
// selene (enemy-selection) frame.
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiSELENE_BASE.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

void aiMIND::ProcessEnemySelection()
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/b-selene", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> timerScope(&localTimeCounter);

    ValidateEnemySystem();
    ValidateSeleneSystem();
    selene.pointee->ProcessFrame();
}
