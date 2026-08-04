#include "psARRAY.h"
#include <cstddef>
#include "psARRAY_DATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
#include "../ds/ds_boundary.h"
#include "../ds/ds_assert_boundary.h"
#include "../../headers/apCL.h"
// psARRAY::psARRAY(const dsDATA&) @ 0x827B00A0 — build an array handle from a boxed dsDATA value:
//   - if `obj` already holds a psARRAY, retrieve it directly (DSD_CONV_RETRIEVE, bumps refcount);
//   - if `obj` holds some other non-null type, allocate a fresh ref-counted backing store and seed
//     its single element from `obj` (via psARRAY_DATA::SetElements, num=1) -- this is the "wrap a
//     scalar value into a 1-element array" coercion path;
//   - if `obj` is empty (null type), this is a contract violation: STRONG_ASSERT crashes (unless
//     suppressed) and the handle is left null.

extern void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug allocator

psARRAY::psARRAY(const dsDATA &obj)
{
    const dsDATA_TYPE *type = obj.type;

    if (type == dsDATA_TYPE_STORAGE<psARRAY>::dataType) {
        this->pData = nullptr;
        obj.GetValue<psARRAY>(*this, DSD_CONV_RETRIEVE);
    } else if (type) {
        psARRAY_DATA *body = static_cast<psARRAY_DATA *>(operator new(
            sizeof(psARRAY_DATA), "D:\\Projects\\code\\common\\src.sys\\ps\\ps_array.cpp", 0x26u));
        if (body) {
            // inlined psARRAY_DATA construction: empty dsVECTOR<dsDATA,8> element store + refCount 1
            apCL cl;
            cl.file = "D:\\Projects\\code\\common\\src.sys\\ps\\ps_array.cpp";
            cl.line = 363;
            body->data.pData = nullptr;
            body->data.nElem = 0;
            body->data.allocated = 0;
            body->data.__cl = cl;
            body->refCount = 1;
            this->pData = body;
            body->SetElements(&obj, 1);
        } else {
            this->pData = nullptr;
            // Deviation from decompiler: rendered as a static `psARRAY_DATA::SetElements(nullptr, ...)`
            // call; SetElements is a non-static member (matches disasm: no null check before the `bl`,
            // so a genuinely null `this` reaches SetElements on OOM) -- reproduced as a null-this call.
            ((psARRAY_DATA *)nullptr)->SetElements(&obj, 1);
        }
    } else if (!IGNORE_STRONG_ASSERT) {
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("0", "D:\\Projects\\code\\common\\src.sys\\ps\\ps_array.cpp", 41, dsStrongAssertMessage);
    }
}
