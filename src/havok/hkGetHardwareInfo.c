#include "../headers/havok/hkHardwareInfo.h"

/* Havok hardware-info query. Reports a fixed 6-thread configuration — the
   Xbox 360 exposes 6 hardware threads (3 cores x 2 SMT), and this port pins
   Havok's worker count to that rather than probing at runtime. */
void hkGetHardwareInfo(hkHardwareInfo *info)
{
    info->m_numThreads = 6;
}
