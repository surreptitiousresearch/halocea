#include "../../headers/ws/ai/aiLOD_SYS.h"

// aiLOD_SYS::UpdateStrikePerfLoad @ 0x83239D78
// ?UpdateStrikePerfLoad@aiLOD_SYS@@IAAXM@Z
//
// Age the strike-performance-load queue by `td` seconds: count every sample's die-timer down,
// erase the ones that have expired, then refresh the cached sum of the survivors' load values.
void aiLOD_SYS::UpdateStrikePerfLoad(float td)
{
    typedef ds::LIST<aiS_PERF_LOAD>::DATA DATA;

    for (DATA *node = queueLoads.head; node; node = node->next)
        node->val.timerToDie = node->val.timerToDie - td;

    DATA *node = queueLoads.head;
    while (node)
    {
        if (node->val.timerToDie >= 0.000001f)
        {
            node = node->next;
        }
        else
        {
            DATA *dead = node;
            node = node->next;
            ds::LIST<aiS_PERF_LOAD>::ITERATOR it;
            it.data = dead;
            queueLoads.Erase(it);
        }
    }

    float sum = 0.0f;
    for (DATA *n = queueLoads.head; n; n = n->next)
        sum = n->val.load + sum;
    cachedLoadSum = sum;
}
