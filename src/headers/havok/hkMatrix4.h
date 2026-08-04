#pragma once
#include "hkVector4.h"
/* size 64 — four column vectors. */
typedef struct hkMatrix4
{
    hkVector4 m_col0;
    hkVector4 m_col1;
    hkVector4 m_col2;
    hkVector4 m_col3;
} hkMatrix4;
