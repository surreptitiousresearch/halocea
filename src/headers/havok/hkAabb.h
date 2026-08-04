#pragma once
#include "hkVector4.h"
/* size 32 — axis-aligned bounding box. */
typedef struct hkAabb
{
    hkVector4 m_min;
    hkVector4 m_max;
} hkAabb;
