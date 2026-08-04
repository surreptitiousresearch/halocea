#pragma once
#include "hkBaseObject.h"

/* size 8 — hkBaseObject base (vtable ptr) + allocation header + refcount. */
#ifdef __cplusplus
struct hkReferencedObject : hkBaseObject
{
    unsigned short m_memSizeAndFlags;  /* offset 4 — allocation size/flags */
    short m_referenceCount;            /* offset 6 */
};
typedef struct hkReferencedObject hkReferencedObject;
#else
/* C has no inheritance: spell the base as the first member. Same size, same offsets. */
typedef struct hkReferencedObject
{
    hkBaseObject base;                 /* offset 0 — vtable pointer */
    unsigned short m_memSizeAndFlags;  /* offset 4 — allocation size/flags */
    short m_referenceCount;            /* offset 6 */
} hkReferencedObject;
#endif

/* boundary refcount ops — Blam-C calls these by name, so both the declaration and the
   definition carry C linkage (the defs got `extern "C"` in 1b06fc932; without it here the
   .cpp gate raises C2732, linkage specification contradicts earlier specification). */
#ifdef __cplusplus
extern "C" {
#endif
void hkReferencedObject_addReference(hkReferencedObject *self);    /* hkReferencedObject::addReference */
void hkReferencedObject_removeReference(hkReferencedObject *self); /* hkReferencedObject::removeReference */
#ifdef __cplusplus
}
#endif
