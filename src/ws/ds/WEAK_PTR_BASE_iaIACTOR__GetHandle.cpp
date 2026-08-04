// ds::WEAK_PTR_BASE<iaIACTOR>::GetHandle @0x823D1BE0 — lazily allocate pHandle {refCount=1,
// pPtr=owning object} on first use. DEVIATION: the raw decompile computes pPtr as `this - 8`,
// i.e. the owning iaIACTOR's `this` recovered from this WEAK_PTR_BASE subobject's own address
// (WEAK_PTR_BASE<iaIACTOR> sits at iaIACTOR+0x20, but `this - 8` here is pointer *word* arithmetic
// on a WEAK_PTR_HANDLE<iaIACTOR>* cast, i.e. -32 bytes = -0x20, exactly undoing that offset) —
// modeled directly as "the iaIACTOR this WEAK_PTR_BASE is embedded in" via a byte-address cast
// rather than replicating the raw pointer-typed subtraction.
#include <cstddef>
#include "../../headers/ws/ds/WEAK_PTR_BASE.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// debug-tracked global placement operator new @0x82506968 (defined in operator_new_dbg.cpp)
void *operator new(size_t, const char *, unsigned int);

template<>
ds::WEAK_PTR_HANDLE<iaIACTOR> *ds::WEAK_PTR_BASE<iaIACTOR>::GetHandle()
{
    if (!pHandle) {
        ds::WEAK_PTR_HANDLE<iaIACTOR> *created =
            (ds::WEAK_PTR_HANDLE<iaIACTOR> *)operator new(sizeof(ds::WEAK_PTR_HANDLE<iaIACTOR>),
                                                            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_weak_ptr.h", 0x31);
        if (created) {
            created->pPtr = (iaIACTOR *)((char *)this - 0x20); // this == &owner->weakSelf
            created->refCount = 1;
        }
        pHandle = created;
    }
    return pHandle;
}
