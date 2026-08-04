#include "../../headers/ws/ai/aiVISION_SNIPER.h"

// aiVISION_SNIPER::MixesLatestToOut @ 0x832921E0
// ?MixesLatestToOut@aiVISION_SNIPER@@MAAXXZ
//
// Sniper override: report the weighted average of the recent test window as the view-percent. The
// latest-test value is read and written back unchanged (a no-op preserved from the binary).
void aiVISION_SNIPER::MixesLatestToOut()
{
    const float savedLatest = viewLatestTest;
    viewPercent    = viewLatestStats.CalcAverage();
    viewLatestTest = savedLatest;
}
