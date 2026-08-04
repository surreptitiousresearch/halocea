#pragma once
#include "hkReferencedObject.h"

/* hkReferencedObject::LockMode (used by the global refcount lock). Only
   LOCK_MODE_AUTO is read at the reversed call sites; the others are present
   for completeness of the enum the DB names. */
typedef enum hkReferencedObject_LockMode
{
    LOCK_MODE_NONE = 0,
    LOCK_MODE_MANUAL = 1,
    LOCK_MODE_AUTO = 2
} hkReferencedObject_LockMode;

/* _RTL_CRITICAL_SECTION is the Win32/CRT type (external boundary). */
typedef struct _RTL_CRITICAL_SECTION _RTL_CRITICAL_SECTION;

/* hkMultiThreadCheck, size 8 (DB types_members). */
#include "hkMultiThreadCheck.h"

/* hkCriticalSection, size 40 (DB types_members). The intrusive SectionList links
   every constructed critical section into a global list for debug enumeration. */
struct hkCriticalSection; /* fwd for the self-referential SectionList links */

/* hkCriticalSection::SectionList, size 12 (DB types_members). */
typedef struct hkCriticalSection_SectionList
{
    int m_spinCount;                          /* 0 */
    struct hkCriticalSection *m_prev;         /* 4 */
    struct hkCriticalSection *m_next;         /* 8 */
} hkCriticalSection_SectionList;

typedef struct hkCriticalSection
{
    unsigned char m_section[28];              /* 0  _RTL_CRITICAL_SECTION */
    hkCriticalSection_SectionList m_list;     /* 28 hkCriticalSection::SectionList */
} hkCriticalSection;

/* hkReferencedObjectLock, size 64 (DB types_members). The hkSingleton<> base at
   offset 8 is zero-size (its static instance pointer lives as a global). */
typedef struct hkReferencedObjectLock
{
    hkReferencedObject base;                  /* 0  */
    hkReferencedObject_LockMode m_lockMode;   /* 8  */
    int m_lockCount;                          /* 12 */
    hkMultiThreadCheck m_multiThreadCheck;    /* 16 */
    hkCriticalSection m_criticalSection;      /* 24 (size 40) -> 64 */
} hkReferencedObjectLock;

void hkReferencedObjectLock_construct(hkReferencedObjectLock *self); /* hkReferencedObjectLock::hkReferencedObjectLock */

/* boundary — the process-wide singleton instance pointer
   (hkSingleton<hkReferencedObjectLock>::s_instance). */
extern hkReferencedObjectLock *hkReferencedObjectLock_getInstance(void);

/* boundary — critical-section primitives */
extern void hkCriticalSection_enter(hkCriticalSection *cs);       /* hkCriticalSection::enter */
extern void hkCriticalSection_leave(hkCriticalSection *cs);       /* RtlLeaveCriticalSection(&cs->m_section) */
extern void hkCriticalSection_construct(hkCriticalSection *cs, int spinCount, int shared); /* hkCriticalSection::hkCriticalSection */
extern void hkMultiThreadCheck_enableChecks(hkMultiThreadCheck *c); /* hkMultiThreadCheck::enableChecks */

/* boundary — Xbox 360 per-thread reentrancy tag. DEVIATION: at the call sites
   this is the raw TLS deref *(int*)(*(int*)r13 + 700) compared against the
   sentinel 601834836 (0x23E5D454); wrapped here as owns/set/clear accessors so
   the recursion-guard logic reads intelligibly. */
extern int  hkRefLock_threadOwns(void);
extern void hkRefLock_setThreadOwns(void);
extern void hkRefLock_clearThreadOwns(void);
