#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_VARS.h"

// aiWEAPON::DbgClearCutOffReasons @ 0x83293E58
// ?DbgClearCutOffReasons@aiWEAPON@@IAAXXZ
//
// Reset the debug "why was this strike's trajectory cut off" reason on every strike-vars block.
void aiWEAPON::DbgClearCutOffReasons()
{
    for (int i = 0; i < svarsList.nEntry; ++i)
        svarsList[i].dbgReason = aiS_VARS::CUT_OFF_NONE;
}
