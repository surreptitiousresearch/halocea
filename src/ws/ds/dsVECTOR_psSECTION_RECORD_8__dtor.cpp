#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary

// dsVECTOR<psSECTION_RECORD,8>::~dsVECTOR @ 0x82517408
// Destroy every live element (dispatching Destroy on each record's type-erased value when
// present) then free the backing storage.
// DEVIATION: the decompiler rendered the per-element teardown as a raw function-pointer call
// `Destroy(value.type)`, i.e. passing the type twice. Disasm shows the real call loads the
// vtable off r3=value.type (the "this") and passes r4 = &element->value (== &value.storage,
// since dsDATA::storage is its first member) as the sole explicit argument — matching
// dsDATA_TYPE::Destroy(unsigned int *storage), the same pattern used in ShrinkResize.
template<>
dsVECTOR<psSECTION_RECORD, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i) {
        psSECTION_RECORD *entry = &this->pData[i];
        if (entry->value.type)
            entry->value.type->Destroy(&entry->value.storage);
    }
    dlFree(this->pData);
}
