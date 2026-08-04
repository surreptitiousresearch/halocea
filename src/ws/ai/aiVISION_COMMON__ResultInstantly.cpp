#include "../../headers/ws/ai/aiVISION_COMMON.h"

// aiVISION_COMMON::ResultInstantly @ 0x83290C20
// ?ResultInstantly@aiVISION_COMMON@@IAAXM@Z
// Protected: force an immediate visibility result — store `val` as the latest test (via the virtual
// StoreLatestTest, vtbl+0x3C) and set the current reported view percent to the same value.
void aiVISION_COMMON::ResultInstantly(float val)
{
    StoreLatestTest(val);
    viewPercent = val;
}
