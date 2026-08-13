#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
#include "psGET_FLAGS.h"
// 0x8251D870 — value stored under `key`: parse it into a complex key, then resolve against the
// body. Empty dsDATA when the handle is null or the key does not split. sret return: the decompiler
// swaps the sret slot with `this` (the real `this` is spelled `result` there). const (QBA).

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

dsDATA psSECTION_INTERFACE::GetValue(const char *key, unsigned int flags) const
{
    dsDATA value;

    if (!this->pData) {
        value.type = nullptr; // empty dsDATA (raw: only the type slot at offset 4 is zeroed)
        return value;
    }

    psCOMPLEX_KEY_DATA complexKey;
    if (this->SplitComplexKey(key, 1, flags & PS_GET_SUBSTR, complexKey)) { // arg4 is a reference
        value = this->pData->GetValue(flags, complexKey);
    } else {
        value.type = nullptr;
    }

    // Release the complex key's scratch split buffer.
    dsTSTRING_BUF_HEADER<char> *buffer = complexKey._psSubStr.pBuffer;
    if (--buffer->refCount == 0)
        dlFree(buffer);

    return value;
}
