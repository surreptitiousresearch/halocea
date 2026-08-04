#pragma once
#include "hkVector4.h"
/* hkContactPoint (32 bytes) — DB types_members. */
typedef struct hkContactPoint
{
    hkVector4 m_position;         /* 0  */
    hkVector4 m_separatingNormal; /* 16 */
} hkContactPoint;
