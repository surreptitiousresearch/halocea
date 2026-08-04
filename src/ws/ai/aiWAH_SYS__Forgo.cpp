#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include <new>

// aiWAH_SYS::Forgo @ 0x831717A8
// ?Forgo@aiWAH_SYS@@QAAXPAVaiWTC_HOLDER@@H@Z
// Drop one reference on `hold`; when its counter reaches zero, delete the holder and remove it from
// `storage`. `idx` is the holder's known storage index; a negative `idx` triggers a linear search
// (no removal if the holder isn't found).
void aiWAH_SYS::Forgo(aiWTC_HOLDER *hold, int idx)
{
    int removeAt = idx;
    if (--hold->counter > 0)
        return;

    if (idx < 0)
    {
        // Locate the holder in storage; leave removeAt negative (skip removal) if not present.
        int nElem = storage.nElem;
        for (int i = 0; i < nElem; ++i)
        {
            if (hold == storage[i])
            {
                removeAt = i;
                break;
            }
        }
    }

    if (removeAt >= 0)
    {
        hold->~aiWTC_HOLDER();
        operator delete(hold);
        storage.Erase(removeAt, 1);
    }
}
