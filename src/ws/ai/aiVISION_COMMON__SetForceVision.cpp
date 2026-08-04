#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/AI_VIST.h"

// aiVISION_COMMON::SetForceVision @ 0x83292608
// ?SetForceVision@aiVISION_COMMON@@UAAXM@Z
//
// Pin the reported view-percent to `value`, drop any pending fresh-result flag, and publish it.
// (The decompiler drops the float argument on the StoreLatestTest call; disasm confirms it forwards
// `value`.)
void aiVISION_COMMON::SetForceVision(float value)
{
    StoreLatestTest(value);
    viewPercent = value;
    state.val &= ~AI_VIST_HAS_FRESH_RESULTS; // ~0x4
    ExportAll();
}
