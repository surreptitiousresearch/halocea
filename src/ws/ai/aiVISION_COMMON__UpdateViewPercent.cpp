#include "../../headers/ws/ai/aiVISION_COMMON.h"

// aiVISION_COMMON::UpdateViewPercent @ 0x83292438
// ?UpdateViewPercent@aiVISION_COMMON@@IAAXXZ
//
// Run the vision test chain unless an instant rule already resolved the result.
void aiVISION_COMMON::UpdateViewPercent()
{
    if (!ApplyInstantRules())
    {
        UpdateLatestTest();
        ConsiderPostingVisRequest();
        MixesLatestToOut();
    }
}
