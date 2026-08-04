#pragma once
#include "../__vector4.h"

/* size 16. types_members hkVector4: a single __vector4 (128-bit VMX register)
   member m_quad; scalar lanes are reached via m_quad.___u0.v[i]. */
typedef struct hkVector4
{
    __vector4 m_quad; /* 0x00 */
} hkVector4;
