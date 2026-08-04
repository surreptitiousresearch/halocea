#include "../../headers/ws/ai/aiFILT_ENEMY.h"

// aiCMP_EDPAIR::cmp(const aiFEE*, const aiFEE*) @ 0x8321F6D8
// ?cmp@aiCMP_EDPAIR@@QBAHABVaiFEE@@0@Z
// strcmp-style ordering by dist4cmp: -1 when a<b, 0 when equal, 1 when a>b.
int aiCMP_EDPAIR::cmp(const aiFEE *a, const aiFEE *b) const
{
    float aw = a->dist4cmp;
    float bw = b->dist4cmp;
    if (aw >= bw)
        return bw < aw; // 1 when a>b, 0 when equal
    return -1;          // a<b
}
