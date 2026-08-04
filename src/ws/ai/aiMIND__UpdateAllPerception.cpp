// aiMIND::UpdateAllPerception @ 0x8322BD48
// ?UpdateAllPerception@aiMIND@@IAAXM@Z  (protected)
//
// Run one perception frame for the brain, under the "b-perc" profiling counter: tick the isa module,
// advance perception by `dt`, post the enemy's visibility request, pick the appropriate vision
// variant, then process vision and the enemy module.
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiISA_BASE.h"
#include "../../headers/ws/ai/aiPERC_BASE.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

void aiMIND::UpdateAllPerception(float dt)
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/b-perc", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> timerScope(&localTimeCounter);

    isa.pointee->ProcessFRAME();
    perc.pointee->ProcessFrame(dt);
    enemy.pointee->PrepareVisibilityRequest();
    DecideAppropriateVision();
    vision.pointee->ProcessFrame();
    enemy.pointee->ProcessFrame();
}
