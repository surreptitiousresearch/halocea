#pragma once
#include "hkError.h"
#include "hkArray.h"
#include "hkObjectArray.h"
#include "hkPointerMap.h"

/* hkdExceptionReporter : hkError — an error sink that captures messages while a
   "catching" scope is active, replacing the global hkError singleton and restoring
   the previous one on stop. Size 44 (DB types_members). */

/* hkdExceptionReporter::ErrorMessage — DB-verified size 104. The full member layout
   (m_type/m_id/m_description/m_file/m_line/m_debugType + six hkObjectArray<Debug*> members)
   pulls a large havok Debug* type graph; sized opaque here since clearMessages only needs the
   104-byte stride for pointer arithmetic and an external element destructor. (2026-07-31) */
typedef struct hkdExceptionReporter_ErrorMessage {
    unsigned char _opaque[104];
} hkdExceptionReporter_ErrorMessage;
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
