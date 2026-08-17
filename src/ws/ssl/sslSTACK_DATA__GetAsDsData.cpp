/* ?GetAsDsData@sslSTACK_DATA@@QBA?AVsslERROR@@AAVdsDATA@@@Z @0x82AF2D20 */
#include "sslSTACK_DATA.h"
#include "sslSTACK.h"
#include "sslCLASS.h"
#include "sslOBJECT.h"
#include "sslOBJ_REF.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AF2D20 — sslSTACK_DATA::GetAsDsData. Read this stack slot out as a dsDATA into `out`. Returns
// sslERROR by value (sret): the decompiler swaps this/result (r3 = return slot sslERROR*, r4 = this
// sslSTACK_DATA*), so its `result->id/desc/tags/[1].id` reads are really this slot's
// type / payload word 0 / word 1 / word 2.
//
// Scalar kinds wrap the payload into a dsDATA of the matching type; reference kinds resolve the
// referent and copy its value out (recursively for a by-stack parameter reference). The reader mirror
// of SetRefTo. const (QBA).
sslERROR sslSTACK_DATA::GetAsDsData(dsDATA &out) const
{
    switch (this->type)
    {
    case TYPE_INT:
        out.StoreValue<int>(this->int_val);
        return sslERROR();

    case TYPE_BOOL:
        out.StoreValue<bool>(this->bool_val);
        return sslERROR();

    case TYPE_FLOAT:
        out.StoreValue<float>(this->float_val);
        return sslERROR();

    case TYPE_DSSTRID:
    {
        // The interned string id lives in the scalar slot; realise it as a dsTSTRING value.
        const dsSTRID *strId = reinterpret_cast<const dsSTRID *>(&this->int_val);
        dsTSTRING<char> text;
        text.pBuffer = nullptr;
        text.UnsafeInit(strId->CStr(), -1, 0);
        out.StoreValue<dsTSTRING<char> >(text);
        return sslERROR();
    }

    case TYPE_DS_DATA:
        out.StoreValue(*reinterpret_cast<const dsDATA *>(&this->data));
        return sslERROR();

    case TYPE_SSLOBJECT_REF:
    {
        sslOBJ_REF ref;
        ref._SetObjectPtr(this->objRef.pObj);
        out.StoreValue<sslOBJ_REF>(ref);
        return sslERROR();
    }

    case TYPE_SSLOBJECT_SUBELEM_REF:
    {
        sslCLASS *cls = this->subElemRef.pObjClass;
        sslOBJECT *obj = this->subElemRef.pObj;
        if (cls && obj)
        {
            if (cls->IsSubElemFunc(this->subElemRef.idx) || cls->IsSubElemEvent(this->subElemRef.idx))
            {
                dsTSTRING<char> msg;
                msg.pBuffer = nullptr;
                msg.UnsafeInit("trying to access function or event as var", -1, 0);
                return sslERROR(SSL_ERR_RUNTIME, msg, 0);
            }
            return cls->GetMemberValue(obj, this->subElemRef.idx, out);
        }
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("trying to access null ref", -1, 0);
        return sslERROR(SSL_ERR_RUNTIME, msg, 0);
    }

    case TYPE_SSLFUNC_PARAM_REF_STACK:
    {
        sslSTACK_DATA &ref = this->funcParamStack.pStack->mainStack[this->funcParamStack.idx];
        return ref.GetAsDsData(out);
    }

    case TYPE_SSLFUNC_PARAM_REF_ARR:
    case TYPE_SSLFUNC_CONST_REF:
        out.StoreValue(this->funcParamArr.pTbl[this->funcParamArr.idx]);
        return sslERROR();

    case TYPE_SSLFUNC_LOCAL_VAR_REF:
        out.StoreValue(this->localVar.pStack->GetLocalVar(this->localVar.idx));
        return sslERROR();

    default: // TYPE_DSDATA_MEMBER_REF or unknown
    {
        if (this->type == TYPE_DSDATA_MEMBER_REF)
        {
            // (A debug strong-assert that dsSTRID(id).CStr() == id is elided; ssl_stack.cpp:512.)
            dsSTRID memberId;
            memberId.id = this->dsDataMemberRef.id;
            const dsDATA_TYPE *structType = (const dsDATA_TYPE *)this->dsDataMemberRef.ds_data[1];
            const unsigned int *structStorage = (const unsigned int *)&this->dsDataMemberRef.ds_data[0];

            if (structType->GetMember(memberId.id, structStorage, &out))
                return sslERROR();

            const char *fromName = structType ? structType->GetDbgName() : "null";
            const char *memberName = memberId.CStr();
            dsTSTRING<char> msg;
            return sslERROR(SSL_ERR_RUNTIME,
                            *dsSPrintf(&msg, "can't retrieve <%s.%s> member", fromName, memberName), 0);
        }

        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("unknown data type in stack", -1, 0);
        return sslERROR(SSL_ERR_GENERIC, msg, 0);
    }
    }
}
