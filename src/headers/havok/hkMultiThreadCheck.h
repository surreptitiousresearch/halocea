#pragma once

/* hkMultiThreadCheck (8 bytes, DB types_members) — debug multithread-access marker.
   Canonical single definition; formerly duplicated inline in hkpWorldObject.h,
   hkpSimulationIsland.h and hkReferencedObjectLock.h (caused C2011 when co-included). */
struct hkMultiThreadCheck
{
    unsigned int m_threadId;          /* 0x00 */
    short m_markCount;                /* 0x04 */
    unsigned short m_markBitStack;    /* 0x06 */
};
typedef struct hkMultiThreadCheck hkMultiThreadCheck;
