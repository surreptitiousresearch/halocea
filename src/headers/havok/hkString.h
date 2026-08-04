#pragma once
#include "hkArray.h"

/* hkString — Havok growable string. It is a single hkArray<char> holding the
   NUL-terminated text; m_data is the buffer, m_size the used length (incl. NUL),
   and the low 30 bits of m_capacityAndFlags are the allocated capacity. */
typedef struct hkString
{
    hkArray<char> m_string; /* 0x00 */
} hkString; /* 12 bytes */
