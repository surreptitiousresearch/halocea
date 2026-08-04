#pragma once

/* Havok hardware-info descriptor. The database resolves only m_numThreads;
   the real Havok SDK struct carries additional fields that this binary never
   references, so only the modelled field is reproduced here. */
typedef struct hkHardwareInfo
{
    int m_numThreads;
} hkHardwareInfo;
