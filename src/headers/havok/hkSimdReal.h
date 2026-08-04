#pragma once
#include "../__vector4.h"

/* hkSimdReal (size 16, DB types_members: single member m_real of type __vector4)
   — a scalar real held in a 128-bit VMX register, splatted across all four lanes.
   Read the scalar from any lane via m_real.___u0.v[i]. */
typedef struct hkSimdReal
{
    __vector4 m_real; /* 0x00 */
} hkSimdReal;
