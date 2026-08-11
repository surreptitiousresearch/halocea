#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"
#include "sslCB_FUNCTION.h"
#include "sslSCRIPTED_FUNCTION.h"
#include "sslSCRIPTED_FUNCTION_BASE.h"
#include "sslEVENT_HANDLER.h"
#include "sslBASE_VAR.h"
#include "sslFUNCTION_BASE.h"
#include "sslSYSTEM.h"
#include "../ds/ds_assert_boundary.h"

// --- other-subsystem boundary (declared, not reversed here) ---
extern "C" void dlFree(void *ptr); // allocator: heap free (dlmalloc-family), same convention as os_boundary.h

// sslCLASS::~sslCLASS() @ 0x82A3AA30
// Tear down every OWNED (non-inherited) element per its TYPE:
//  - TYPE_CB_FUNC (0): a single native callback binding -- base-dtor + delete.
//  - TYPE_SSL_FUNC (1): a single compiled function -- base-dtor + delete.
//  - TYPE_EVENT (2): a chain of sslEVENT_HANDLER nodes (linked via pNext); delete each node owned
//    by this class, stopping (without unlinking) at the first node whose pClassOfDefinition
//    differs -- the remainder belongs to a base class and is torn down when that class is deleted.
//  - anything else reaching this class's runtime (3, 4, or TYPE_CONST==5): a polymorphic
//    sslBASE_VAR -- a plain `delete` dispatches through its vtable's deleting destructor (the
//    decompile's raw `(**vtbl)(vtbl, 1)` call). The decompiler's `type == 3 && v4->type || type ==
//    4 && v4->type || v4->type` condition is exactly `type == 3 || type == 4 || type == TYPE_CONST`
//    (the `&& v4->type` conjuncts are tautological once `type` is known non-zero and equal to 3/4;
//    reproduced here in its simplified form).
// Frees the bit-events table if the class allocated one (stateClass bit 0x2), decrements the
// live-class counter and notifies the owning system, and releases dbgInfo's string buffer.
//
// CAVEAT: the decompile also shows the events/elements/nameToElemIdx dsVECTOR backing arrays
// freed directly here (`dlFree(vec.pData)`, each preceded by a no-op loop -- all three hold
// trivially-destructible element types), and an explicit `sslCLASS_REF::~sslCLASS_REF`/
// `sslSOURCE_REF::~sslSOURCE_REF` call for refParent/source. Every one of those five members
// (events, elements, nameToElemIdx, source, refParent) is a real member with its own
// already-reversed, non-trivial destructor, so in this C++ reconstruction the compiler-generated
// member epilogue destroys all five automatically (in reverse declaration order) once this body
// returns; writing the same teardown explicitly here as well would double-free/double-release
// them. The apparent inline dlFree/explicit-dtor-call sequence in the decompile is simply what
// that automatic epilogue compiles down to (the dsVECTOR destructor is small enough to inline;
// sslCLASS_REF/sslSOURCE_REF's were not). Nothing further is written for these five members.
sslCLASS::~sslCLASS()
{
    static const char *const kSslClassCpp = "D:\\Projects\\code\\common\\src.sys\\ssl\\ssl_class.cpp";

    for (int elementIndex = 0; elementIndex < this->elements.nElem; ++elementIndex)
    {
        sslCLASS_ELEMENT &element = this->elements[elementIndex];
        if (element.isInherited)
            continue;

        unsigned char type = element.type;
        if (type > 5)
        {
            if (!IGNORE_STRONG_ASSERT)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0", kSslClassCpp, 167, empty_string); // instance-call, 4 args
        }
        else if (type == sslCLASS_ELEMENT::TYPE_SSL_FUNC)
        {
            if (element.pVoid)
                delete element.pSslFunc; // dtor call + operator delete, matching the decompile's split form
        }
        else if (type == sslCLASS_ELEMENT::TYPE_EVENT)
        {
            sslEVENT_HANDLER *handler = reinterpret_cast<sslEVENT_HANDLER *>(element.pSslFunc);
            while (handler)
            {
                if (handler->pClassOfDefinition != this)
                    break;
                sslEVENT_HANDLER *next = handler->pNext;
                delete handler;
                handler = next;
            }
        }
        else if (type == 3 || type == 4 || type == sslCLASS_ELEMENT::TYPE_CONST)
        {
            // sslBASE_VAR.h models __vftable as a raw pointer (not an idiomatic C++ virtual
            // class), so dispatch its deleting destructor (vtable slot 0x00) manually, matching
            // the decompile's raw `(**v10)(v10, 1)` call -- a plain `delete` would not go through
            // the vtable here.
            if (element.pVoid)
            {
                // DEVIATION 2026-08-07: was a reinterpret_cast to `void (*)(sslBASE_VAR *, int)`,
                // because sslBASE_VAR.h copied the DB's single-argument slot type. ??_7sslBASE_VAR@@6B@+0x00
                // holds ??_EsslBASE_VAR@@UAAPAXI@Z, so the slot takes the flag and the cast is gone.
                element.pVar->__vftable->dtr_sslBASE_VAR(element.pVar, 1); // vtable slot 0 fn ptr
            }
        }
        else // TYPE_CB_FUNC (0)
        {
            if (element.pVoid)
                delete element.pCbFunc; // dtor call + operator delete, matching the decompile's split form
        }
    }

    if ((this->stateClass.state & 2) != 0)
    {
        operator delete(this->pBitEventsTbl);
        this->pBitEventsTbl = nullptr;
    }

    --__sslNClass;
    if (this->pSslSystem)
        this->pSslSystem->_OnClassDelete(this->uid); // member-call form

    dsTSTRING_BUF_HEADER<char> *dbgInfoBuffer = this->dbgInfo.pBuffer;
    if (dbgInfoBuffer->refCount-- == 1)
        dlFree(dbgInfoBuffer);

    // this->events, this->elements, this->nameToElemIdx, this->source, and this->refParent are all
    // destroyed automatically here (compiler-generated member epilogue, reverse declaration order)
    // -- see the CAVEAT above.
}
