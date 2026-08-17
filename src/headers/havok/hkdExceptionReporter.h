#pragma once
#include "hkError.h"
#include "hkArray.h"
#include "hkObjectArray.h"
#include "hkPointerMap.h"

/* hkdExceptionReporter : hkError — an error sink that captures messages while a
   "catching" scope is active, replacing the global hkError singleton and restoring
   the previous one on stop. Size 44 (DB types_members). */

/* hkdExceptionReporter::ErrorMessage — DB-verified size 104. Canonical typed layout
   in hkdExceptionReporter_ErrorMessage.h (was restated opaque here; the ODR duplicate
   made the bulk header probe drop both definitions). */
#include "hkdExceptionReporter_ErrorMessage.h"
typedef struct hkdExceptionReporter_vtbl hkdExceptionReporter_vtbl;                  /* boundary vtable */

typedef struct hkdExceptionReporter
{
    hkError               base;                /* 0x00 */
    hkObjectArray<hkdExceptionReporter_ErrorMessage> m_messages; /* 0x08 */
    hkPointerMap<int,int> m_disabledAssertIds; /* 0x14 */
    hkArray<int>          m_sectionIds;        /* 0x20 */
} hkdExceptionReporter;                        /* 0x2C (44) */

/* ReportType passed to stopCatching (0 = do not report). DB exposes the name. */
typedef int hkdExceptionReporter_ReportType;

extern void *const hkdExceptionReporter_vftable;      /* hkdExceptionReporter::`vftable' */
extern hkError *hkdExceptionReporter_s_previousErrorHandler; /* hkdExceptionReporter::s_previousErrorHandler */

void hkdExceptionReporter_startCatching(void);                                  /* hkdExceptionReporter::startCatching */
void hkdExceptionReporter_stopCatching(hkdExceptionReporter_ReportType report); /* hkdExceptionReporter::stopCatching */
