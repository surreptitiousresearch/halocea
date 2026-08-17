/* ?Cleanup@sslSTACK_DATA@@IAAXXZ @0x82AF05F8 */
#include "sslSTACK_DATA.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "../ds/dsDATA.h"

// Global atomic-decrement helper; body not reversed here.
extern "C" long osLockedDecrement(volatile long *pValue); // boundary

// 0x82AF05F8 — sslSTACK_DATA::Cleanup. Drop the current payload, leaving the slot empty. Only the
// reference-holding and boxed-value arms need teardown; scalar (int/bool/float/strid) and plain
// index arms (TYPE_SSLFUNC_*_REF) carry nothing to release.
void sslSTACK_DATA::Cleanup()
{
    // Fast reject: only tags in [TYPE_DS_DATA .. TYPE_DSDATA_MEMBER_REF] own resources.
    if ((unsigned int)(this->type - TYPE_DS_DATA) > 7u)
        return;

    if (this->type == TYPE_SSLOBJECT_REF || this->type == TYPE_SSLOBJECT_SUBELEM_REF)
    {
        // objRef/subElemRef: pObj @+4 and pClass/pObjClass @+8 each carry a dsREF_COUNT at offset 0.
        if (this->objRef.pObj)
            osLockedDecrement((volatile long *)&this->objRef.pObj->refCount.refCount);
        if (this->objRef.pClass)
            osLockedDecrement((volatile long *)&this->objRef.pClass->refCount.refCount);
    }
    else if (this->type != TYPE_SSLFUNC_PARAM_REF_STACK
          && this->type != TYPE_SSLFUNC_PARAM_REF_ARR
          && this->type != TYPE_SSLFUNC_CONST_REF
          && this->type != TYPE_SSLFUNC_LOCAL_VAR_REF)
    {
        // TYPE_DS_DATA / TYPE_DSDATA_MEMBER_REF: an in-place dsDATA occupies the payload (storage@+4,
        // type@+8). Dispatch the runtime type's Destroy on the stored word.
        dsDATA *value = reinterpret_cast<dsDATA *>(&this->data);
        if (value->type)
            // Decompiler rendered this as a 1-arg indirect call through vtable slot 0xC; Destroy
            // actually takes the storage pointer in r4 (same shape as dsVECTOR<LOCAL_VAR>::Resize).
            value->type->Destroy(&value->storage);
    }
}
