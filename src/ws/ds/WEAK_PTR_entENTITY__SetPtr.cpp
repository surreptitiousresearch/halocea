// ds::WEAK_PTR<entENTITY>::SetPtr @0x823D575C — Release() the current handle, then, if `pPtr` is
// non-null, adopt pPtr's weak handle (bumping its refcount). DEVIATION: the raw decompile
// resolves the new handle via `ds::WEAK_PTR_BASE<iaIACTOR>::GetHandle(&pPtr->WEAK_PTR_BASE<iaIACTOR>)`
// — entENTITY's weak identity is provided by its iaIACTOR base's WEAK_PTR_BASE<iaIACTOR>
// sub-object, not a WEAK_PTR_BASE<entENTITY> of its own (entENTITY derives iaIACTOR; see
// ../ia/iaIACTOR.h). The resulting WEAK_PTR_HANDLE<iaIACTOR>* is reinterpreted as
// WEAK_PTR_HANDLE<entENTITY>* (same layout: refCount, pPtr), matching the binary exactly.
#include "../../headers/ws/ds/WEAK_PTR.h"
#include "../../headers/ws/ds/WEAK_PTR_BASE.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern "C" int osLockedDecrement(volatile int *slot);
extern "C" int osLockedIncrement(volatile int *slot);

struct entENTITY : iaIACTOR {}; // boundary — full entENTITY layout is out of scope here; only the
                                 // inherited WEAK_PTR_BASE<iaIACTOR> sub-object is used

template<>
void ds::WEAK_PTR<entENTITY>::SetPtr(entENTITY *pPtr)
{
    Release();
    if (pPtr) {
        ds::WEAK_PTR_HANDLE<entENTITY> *handle =
            (ds::WEAK_PTR_HANDLE<entENTITY> *)pPtr->weakSelf.GetHandle();
        pHandle = handle;
        osLockedIncrement(&handle->refCount);
    }
}
