#include "../../headers/ws/rend/rendSR_DATA.h"

// 0x829ECED0 — copy the position and per-lamp data vector from src; pLock/priorClass/priorCur
// are per-instance runtime state and are intentionally left untouched.
void rendSR_DATA::Copy(const rendSR_DATA &src)
{
    pos = src.pos;
    lampsData = src.lampsData;
}
