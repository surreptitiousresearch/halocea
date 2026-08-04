#pragma once
#include "hkError.h"
#include "hkArray.h"
#include "hkBool.h"
#include "hkPointerMap.h"
#include "hkPointerMapBase.h"

/* hkDefaultError : hkError — Havok's default error/assert sink, size 40 (DB
   types_members). Suppressed assert ids live in m_disabledAssertIds; enabled
   sections in m_sectionIds; an optional user callback receives the formatted
   text. */
typedef struct hkDefaultError
{
    hkError               base;                /* 0x00 */
    hkPointerMap<int,int> m_disabledAssertIds; /* 0x08 */
    hkArray<int>          m_sectionIds;        /* 0x14 */
    void (*m_errorFunction)(const char *, void *); /* 0x20 */
    void                 *m_errorObject;       /* 0x24 */
} hkDefaultError;

void hkDefaultError_setEnabled(hkDefaultError *self, unsigned int id, hkBool *enabled); /* hkDefaultError::setEnabled */
