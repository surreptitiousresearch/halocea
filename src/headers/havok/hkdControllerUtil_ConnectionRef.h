#pragma once
#include "hkdBreakableShape_Connection.h"

/* hkdControllerUtil::ConnectionRef, size 8. A lightweight reference produced by
   findConnections: a pointer to the shared Connection plus the two child ids.
   Verified against DB types_members. */
typedef struct hkdControllerUtil_ConnectionRef
{
    const Connection *m_connection;                   /* 0 — DB const hkdBreakableShape::Connection * */
    unsigned short m_idA;                             /* 4 */
    unsigned short m_idB;                             /* 6 */
} hkdControllerUtil_ConnectionRef;
