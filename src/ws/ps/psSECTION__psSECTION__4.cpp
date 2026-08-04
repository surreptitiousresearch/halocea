#include "psSECTION.h"
#include "../ds/dsDATA.h"
#include "../ds/ds_boundary.h"
// psSECTION::psSECTION(const dsDATA&) @ 0x8251ABA0 — extract a referenced section handle out of a
// boxed dsDATA value: null handle unless `obj` currently holds a psSECTION, in which case retrieve
// it via dsDATA::GetValue (DSD_CONV_RETRIEVE), which bumps the section's refcount on the way out.

psSECTION::psSECTION(const dsDATA &obj)
{
    this->pData = nullptr;
    if (obj.type == dsDATA_TYPE_STORAGE<psSECTION>::dataType)
        obj.GetValue<psSECTION>(*this, DSD_CONV_RETRIEVE);
}
