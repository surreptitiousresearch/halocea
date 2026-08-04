#pragma once
#include "hkVector4.h"

typedef struct hkdBreakableShape hkdBreakableShape;

/* hkdBreakableShape::Connection, size 64. Verified against DB types_members. */
typedef struct hkdBreakableShape_Connection
{
    hkVector4 m_pivotA;            /* 0  */
    hkVector4 m_pivotB;            /* 16 */
    hkVector4 m_separatingNormal;  /* 32 */
    float m_contactArea;           /* 48 */
    const hkdBreakableShape *m_a;  /* 52 */
    const hkdBreakableShape *m_b;  /* 56 */
    unsigned char _pad3C[4];       /* 60 -> 64 */
} hkdBreakableShape_Connection;

/* The DB spells this nested type hkdBreakableShape::Connection; member declarations
   elsewhere reference the unqualified nested name. */
typedef hkdBreakableShape_Connection Connection;
