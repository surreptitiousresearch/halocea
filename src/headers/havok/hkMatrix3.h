#pragma once
#include "hkVector4.h"

/* size 48 — column-major 3x3, each column padded to a full hkVector4. */
typedef struct hkMatrix3
{
    hkVector4 m_col0;
    hkVector4 m_col1;
    hkVector4 m_col2;
} hkMatrix3;
