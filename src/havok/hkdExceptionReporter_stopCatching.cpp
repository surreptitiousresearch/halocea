/* ?stopCatching@hkdExceptionReporter@@SAXW4ReportType@1@@Z @0x8386CC90 */
#include "../headers/havok/hkdExceptionReporter.h"
#include "../headers/havok/hkReferencedObject.h"

/* boundary — message flush/clear and the locked refcount drop. */
extern void hkdExceptionReporter_reportMessages(int a, hkdExceptionReporter_ReportType report, int b); /* hkdExceptionReporter::reportMessages */
extern void hkdExceptionReporter_clearMessages(void);                                 /* hkdExceptionReporter::clearMessages */
extern void hkReferencedObject_removeReferenceLockUnchecked(hkReferencedObject *self); /* hkReferencedObject::removeReferenceLockUnchecked */

/* hkdExceptionReporter::stopCatching — optionally report the captured messages,
   clear them, then restore the previously-installed global hkError handler. */
void hkdExceptionReporter_stopCatching(hkdExceptionReporter_ReportType report)
{
    hkError *previous;

    if (report)
        hkdExceptionReporter_reportMessages(0, report, 1);
    hkdExceptionReporter_clearMessages();

    previous = hkdExceptionReporter_s_previousErrorHandler;
    if (hkError_s_instance)
        hkReferencedObject_removeReferenceLockUnchecked((hkReferencedObject *)hkError_s_instance);
    hkError_s_instance = previous;
    hkdExceptionReporter_s_previousErrorHandler = nullptr;
}
