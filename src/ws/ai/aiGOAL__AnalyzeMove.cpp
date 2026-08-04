// aiGOAL::AnalyzeMove @ 0x832AF6A8
// ?AnalyzeMove@aiGOAL@@UAAX_N@Z
#include "../../headers/ws/ai/aiGOAL.h"

// Per-frame movement analysis: refresh pos-reached + stuck flags/event, and (2-D) touch reaction.
void aiGOAL::AnalyzeMove(bool is2D)
{
    UpdatePosReached(is2D);
    UpdateStuckFlags();
    UpdateStuckEvent();
    if (is2D)
        ReactToTouch();
}
