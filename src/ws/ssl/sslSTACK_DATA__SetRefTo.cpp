/* ?SetRefTo@sslSTACK_DATA@@QAA?AVsslERROR@@ABVdsDATA@@@Z @0x82AF27D0 */
#include "sslSTACK_DATA.h"
#include "sslSTACK.h"
#include "sslCLASS.h"
#include "sslOBJECT.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AF27D0 — sslSTACK_DATA::SetRefTo. Assign `val` through this reference-kind stack slot. Returns
// sslERROR by value (sret): the decompiler swaps this/result (r3 = return slot sslERROR*, r4 = this
// sslSTACK_DATA*), so its `result->id/desc/tags/[1].id` reads are really this slot's
// type / payload word 0 / word 1 / word 2.
//
// Only reference kinds (discriminators 5..11) are assignable. Object and r-value / parameter refs are
// read-only; sub-element and local-var and struct-member refs forward the store to the underlying
// container. A non-reference slot yields an "assign to r_value" error.
sslERROR sslSTACK_DATA::SetRefTo(const dsDATA &val)
{
    sslERROR result; // no-error accumulator (returned by the forwarding cases)

    if ((unsigned int)(this->type - TYPE_SSLOBJECT_REF) > 6)
    {
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("trying to assign data to r_value", -1, 0);
        return sslERROR(SSL_ERR_GENERIC, msg, 0);
    }

    switch (this->type)
    {
    case TYPE_SSLOBJECT_REF: // the built-in $object reference is read-only
    {
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("can't change '$object' value", -1, 0);
        return sslERROR(SSL_ERR_GENERIC, msg, 0);
    }

    case TYPE_SSLOBJECT_SUBELEM_REF:
    {
        sslOBJECT *obj = this->subElemRef.pObj;
        if (!obj)
        {
            dsTSTRING<char> msg;
            msg.pBuffer = nullptr;
            msg.UnsafeInit("trying to assign data by null ref", -1, 0);
            result = sslERROR(SSL_ERR_GENERIC, msg, 0);
        }
        else if (((obj->state.state >> 4) & 1) == 0) // skip silently while the object's guard bit (0x10) is set
        {
            result = this->subElemRef.pObjClass->SetMemberValue(obj, this->subElemRef.idx, val);
        }
        return result;
    }

    case TYPE_SSLFUNC_PARAM_REF_STACK:
    case TYPE_SSLFUNC_PARAM_REF_ARR:
    {
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("can't change function parameter value", -1, 0);
        return sslERROR(SSL_ERR_GENERIC, msg, 0);
    }

    case TYPE_SSLFUNC_CONST_REF: // r-value / constant reference
    {
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("trying to assign data to r_value", -1, 0);
        return sslERROR(SSL_ERR_GENERIC, msg, 0);
    }

    case TYPE_SSLFUNC_LOCAL_VAR_REF:
        return this->localVar.pStack->SetLocalVar(this->localVar.idx, val);

    default: // TYPE_DSDATA_MEMBER_REF
    {
        // The referenced value is the in-place dsDATA at the head of the member ref; ds_data[1] is
        // its runtime type and ds_data[0] is its storage word. (A debug strong-assert that
        // dsSTRID(id).CStr() == id is elided; ssl_stack.cpp:409.)
        dsSTRID memberId;
        memberId.id = this->dsDataMemberRef.id;
        const dsDATA_TYPE *structType = (const dsDATA_TYPE *)this->dsDataMemberRef.ds_data[1];
        unsigned int *structStorage = (unsigned int *)&this->dsDataMemberRef.ds_data[0];

        if (!structType->SetMember(memberId.id, structStorage, &val))
        {
            const char *fromName = structType ? structType->GetDbgName() : "null";
            const char *memberName = memberId.CStr();
            dsTSTRING<char> msg;
            return sslERROR(SSL_ERR_RUNTIME,
                            *dsSPrintf(&msg, "can't assign <%s.%s> member", fromName, memberName), 0);
        }
        return result;
    }
    }
}
