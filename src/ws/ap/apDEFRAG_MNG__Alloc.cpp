// apDEFRAG_MNG::Alloc<T>() @0x823F1968 (instantiated for T=vidDEFRAG_CHUNK_OBJ_HCEX_X360) —
// heap-allocate a T (installing T's vtable over the zeroed apDEFRAG_CHUNK base fields, matching
// T's own default ctor), then AllocImpl() it into the matching pool/descriptor; on failure (either
// the allocation or AllocImpl), tear the chunk back down via its own scalar-deleting-dtor slot and
// return nullptr.
#include "../../headers/ws/ap/apDEFRAG_MNG.h"

template<class T>
T *apDEFRAG_MNG::Alloc(int size)
{
    T *chunk = new T(); // T's own ctor zeroes the apDEFRAG_CHUNK base and installs T's vtable
    // T embeds apDEFRAG_CHUNK as its first member (offset 0), so a reinterpret to the base is valid.
    if (AllocImpl(reinterpret_cast<apDEFRAG_CHUNK *>(chunk), size))
        return chunk;
    if (chunk) {
        apDEFRAG_CHUNK *base = reinterpret_cast<apDEFRAG_CHUNK *>(chunk); // dispatch through base vtable slot
        base->__vftable->dtr_apDEFRAG_CHUNK(base, 1); // scalar-deleting dtr: deleteFlag=1 => free
    }
    return nullptr;
}

#include "../../headers/hcex/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"
template vidDEFRAG_CHUNK_OBJ_HCEX_X360 *apDEFRAG_MNG::Alloc<vidDEFRAG_CHUNK_OBJ_HCEX_X360>(int size);
