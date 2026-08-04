// aiCHEAP_VISION::ProcessFRAME @ 0x8323FBE8
// ?ProcessFRAME@aiCHEAP_VISION@@QAAXM@Z
#include "../../headers/ws/ai/aiCHEAP_VISION.h"

// Age every tracked enemy's cached sightings by `delta`.
void aiCHEAP_VISION::ProcessFRAME(float delta)
{
    for (ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *>::DATA *node = listVIE.head; node; node = node->next)
        node->val->ProcessFrame(delta);
}
