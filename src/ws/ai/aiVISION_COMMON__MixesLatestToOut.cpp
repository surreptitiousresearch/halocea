#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiVISION_COMMON::MixesLatestToOut @ 0x83291888
// ?MixesLatestToOut@aiVISION_COMMON@@MAAXXZ
//
// Relax the reported view-percent toward the latest test result. When the target is already at
// least as visible as the reported value, decay toward it over the lost-sight time; otherwise raise
// it using the descriptor's distance->raising-speed curve, clamping so it never overshoots.
void aiVISION_COMMON::MixesLatestToOut()
{
    const float dt = pBrain->GetElapsedTime();
    aiSTATUS *st = pBrain->st.pointee;

    float result;
    if (viewLatestTest <= viewPercent)
    {
        const float lostTime = CalculateLostSightTime(st->enemy.dist);
        const float ratio    = dt / lostTime;
        result = (1.0f - ratio) * viewPercent + viewLatestTest * ratio;
        if (result >= 100.0f)
            result = 100.0f;
    }
    else
    {
        // The raising-speed curve writes its value into element 0 (seeded to 100 as the default).
        float raisingSpeed[2];
        raisingSpeed[0] = 100.0f;
        st->perc.viewPercRaisingSpeed.GetValue(st->enemy.dist, raisingSpeed, nullptr);
        result = viewLatestTest;
        const float raised = raisingSpeed[0] * dt + viewPercent;
        if (raised <= viewLatestTest)
        {
            viewPercent = raised;
            return;
        }
    }
    viewPercent = result;
}
