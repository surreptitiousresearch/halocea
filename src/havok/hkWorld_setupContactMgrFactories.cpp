/* hkWorld_setupContactMgrFactories @0x838F6300 */
#include "../headers/havok/hkpContactMgr_boundary.h"
#include "../headers/havok/hkThreadMemory.h"

/* Install the three standard contact-manager factories into the collision
   dispatcher: simple-constraint (type 1), report (type 2) and null (type 3).
   Each is allocated, stamped with its allocation size, constructed, registered,
   then released (the dispatcher retains its own reference). */
void hkWorld_setupContactMgrFactories(hkpWorld *world, hkpCollisionDispatcher *dispatcher)
{
    void *simpleChunk;
    void *reportChunk;
    hkpContactMgrFactory *simpleFactory;
    hkpContactMgrFactory *reportFactory;
    hkpContactMgrFactory *nullFactory;

    /* simple-constraint contact-manager factory */
    simpleChunk = hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(), 12, HK_MEMORY_CLASS_CONTACT);
    ((hkReferencedObject *)simpleChunk)->m_memSizeAndFlags = 12;
    /* DEVIATION: the decompiler fused the chunk arg and rendered the ctor result
       as (u64)Factory() >> 32 (reg-alloc __PAIR64__ artifact); the factory is
       simply the constructed chunk. */
    simpleFactory = hkpSimpleConstraintContactMgr_Factory_construct(simpleChunk);

    /* report contact-manager factory (bound to the world) */
    reportChunk = hkThreadMemory_allocateChunk(hkThreadMemory_getCurrent(), 12, HK_MEMORY_CLASS_CONTACT);
    ((hkReferencedObject *)reportChunk)->m_memSizeAndFlags = 12;
    reportFactory = hkpReportContactMgr_Factory_construct(reportChunk, world);

    /* null contact-manager factory (inline-initialised, 20 bytes: factory header
       + an embedded prototype null-manager at +8 + a type tag at +16) */
    nullFactory = (hkpContactMgrFactory *)hkThreadMemory_allocateChunk(
        hkThreadMemory_getCurrent(), 20, HK_MEMORY_CLASS_DYNAMICS);
    nullFactory->base.m_referenceCount = 1;
    nullFactory->base.__vftable = &hkpNullContactMgrFactory_vftable;
    nullFactory->base.m_memSizeAndFlags = 20;
    *(hkBaseObject_vtbl **)((char *)nullFactory + 8) = &hkpNullContactMgr_vftable;
    *(short *)((char *)nullFactory + 14) = 1;
    *(int *)((char *)nullFactory + 16) = 3;

    hkpCollisionDispatcher_registerContactMgrFactoryWithAll(dispatcher, simpleFactory, 1);
    hkpCollisionDispatcher_registerContactMgrFactoryWithAll(dispatcher, reportFactory, 2);
    hkpCollisionDispatcher_registerContactMgrFactoryWithAll(dispatcher, nullFactory, 3);

    hkReferencedObject_removeReference((hkReferencedObject *)simpleFactory);
    hkReferencedObject_removeReference((hkReferencedObject *)reportFactory);
    hkReferencedObject_removeReference((hkReferencedObject *)nullFactory);
}
