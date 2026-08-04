#include "sslSTACK_DATA.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"

// Global placement copy-construct helper (New<T>(dst, src)); body not reversed here.
template<class T> T *New(T *dst, const T *src); // boundary

// 0x82AF3D50 — sslSTACK_DATA::ConvertToDsData. Convert this stack value in place to a TYPE_DS_DATA
// holding its dsDATA form. A no-op (returns no-error) when already TYPE_DS_DATA. On a conversion
// error, leaves the slot untouched and returns the error. Returns sslERROR by value (sret): the
// decompiler swaps this/result (r3 = return slot, r4 = this sslSTACK_DATA*).
sslERROR sslSTACK_DATA::ConvertToDsData()
{
    if (this->type == TYPE_DS_DATA)
        return sslERROR();

    dsDATA value;
    value.type = nullptr;
    sslERROR err = this->GetAsDsData(value); // temp GetAsDsData return copied into `err`, then dropped

    sslERROR result;
    if (err.id)
    {
        // Conversion failed: propagate the error unchanged.
        result = err;
    }
    else
    {
        // Success: replace this slot's payload with the produced dsDATA.
        this->Cleanup();
        this->type = TYPE_DS_DATA;
        New<dsDATA>(reinterpret_cast<dsDATA *>(&this->data), &value);
        result = sslERROR();
    }

    if (value.type)
        value.type->Destroy(&value.storage);
    return result;
}
