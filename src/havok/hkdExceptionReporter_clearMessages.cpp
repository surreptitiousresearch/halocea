#include "../headers/havok/hkdExceptionReporter.h"
#include "../headers/havok/hkError.h"

/* boundary — element destructor (104-byte hkdExceptionReporter::ErrorMessage). */
extern void hkdExceptionReporter_ErrorMessage_destruct(hkdExceptionReporter_ErrorMessage *self); /* ErrorMessage::~ErrorMessage */

/* hkdExceptionReporter::clearMessages — destroy every captured message and reset
   the message array to empty. The active reporter is the current global hkError
   singleton reinterpreted as an hkdExceptionReporter; its m_messages array holds
   fixed 104-byte ErrorMessage records. Storage capacity is left intact; only the
   element count is zeroed. */
void hkdExceptionReporter_clearMessages(void)
{
    hkdExceptionReporter *reporter = (hkdExceptionReporter *)hkError_s_instance;
    hkdExceptionReporter_ErrorMessage *msg =
        (hkdExceptionReporter_ErrorMessage *)reporter->m_messages.m_data;
    int i = reporter->m_messages.m_size;

    for (; i > 0; --i)
    {
        hkdExceptionReporter_ErrorMessage_destruct(msg);
        ++msg; /* stride 104 == sizeof(hkdExceptionReporter_ErrorMessage), DB-verified */
    }

    reporter->m_messages.m_size = 0;
}
