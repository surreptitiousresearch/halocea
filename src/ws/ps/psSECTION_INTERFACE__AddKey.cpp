/* ?AddKey@psSECTION_INTERFACE@@QAAHPBDVdsDATA@@KH@Z @0x8251D798 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
// 0x8251D798 — install `val` under `key` (creating an empty body first if null). Parses `key` into
// a complex key, copies `val` into a scratch dsDATA, and forwards to the body. Returns the leaf
// record index or -1. Consumes the by-value `val`.

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

int psSECTION_INTERFACE::AddKey(const char *key, dsDATA val, unsigned int flags, int lineNmb)
{
    if (!this->pData)
        this->CreateEmpty();

    psCOMPLEX_KEY_DATA complexKey;
    this->SplitComplexKey(key, 0, 0, complexKey); // arg4 is a reference (was &complexKey)

    dsDATA value;
    value.type = nullptr;
    value.StoreValue(val);

    int recIdx = this->pData->AddKey(value, flags, lineNmb, complexKey);

    // Release the complex key's scratch split buffer.
    dsTSTRING_BUF_HEADER<char> *buffer = complexKey._psSubStr.pBuffer;
    if (--buffer->refCount == 0)
        dlFree(buffer);

    // Consume the by-value `val`.
    if (val.type)
        val.type->Destroy(&val.storage);

    return recIdx;
}
