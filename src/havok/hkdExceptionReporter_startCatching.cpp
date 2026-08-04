#include "../headers/havok/hkdExceptionReporter.h"
#include "../headers/havok/hkPointerMapBase.h"
#include "../headers/havok/hkReferencedObject.h"
#include "../headers/havok/hkThreadMemory.h"

/* boundary — the empty-map default constructor (a distinct overload from the
   buffer-adopting hkPointerMapBase_construct) and the locked refcount drop. */
extern void hkPointerMapBase_construct_default(hkPointerMapBase *self); /* hkPointerMapBase::hkPointerMapBase() */
extern void hkReferencedObject_removeReferenceLockUnchecked(hkReferencedObject *self); /* hkReferencedObject::removeReferenceLockUnchecked */

/* hkdExceptionReporter::startCatching — allocate a fresh reporter (44 bytes),
   initialise it with empty message/disabled-id/section arrays, and swap it in as
   the global hkError singleton, saving the previous handler for stopCatching. */
void hkdExceptionReporter_startCatching(void)
{
    /* DEVIATION: allocator fetched at the call site as the opaque TLS deref
       *(hkThreadMemory**)(*(int*)r13 + 704); modeled via the accessor. */
    hkdExceptionReporter *reporter = (hkdExceptionReporter *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 44, HK_MEMORY_CLASS_DESTRUCTION);

    reporter->base.base.m_memSizeAndFlags = 44;
    reporter->base.base.__vftable = (hkBaseObject_vtbl *)&hkdExceptionReporter_vftable;
    reporter->base.base.m_referenceCount = 1;

    reporter->m_messages.m_data = 0;
    reporter->m_messages.m_size = 0;
    reporter->m_messages.m_capacityAndFlags = (int)0x80000000;

    hkPointerMapBase_construct_default((hkPointerMapBase *)&reporter->m_disabledAssertIds.m_map);

    reporter->m_sectionIds.m_data = 0;
    reporter->m_sectionIds.m_size = 0;
    reporter->m_sectionIds.m_capacityAndFlags = (int)0x80000000;

    hkdExceptionReporter_s_previousErrorHandler = hkError_s_instance;
    hkReferencedObject_addReference((hkReferencedObject *)hkError_s_instance);
    if (hkError_s_instance)
        hkReferencedObject_removeReferenceLockUnchecked((hkReferencedObject *)hkError_s_instance);
    hkError_s_instance = (hkError *)reporter;
}
