#include "MAP.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "dsTSTRING_BUF_HEADER.h"
#include "dsSTRID.h"

// ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Insert(ITEM**,K&,V&)
// @ 0x823B95A8
// Private bucket-splice helper: the caller (public Insert, not reversed) has already located the
// bucket chain head `*item`; this allocates one ITEM from the node pool, copies key/value in, bumps
// the shared string-buffer refcount for `value.filePath`, and links the node onto the chain head.
// On allocator exhaustion it stores `*item` through a null pointer (`MEMORY[0] = *item` in the
// decompile) rather than checking failure — reproduced faithfully as a null-pointer write, matching
// the disassembly's unconditional store; this mirrors Halo's original crash-on-OOM behavior for the
// node-pool allocator's exhausted path.
template<>
void ds::MAP<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Insert(
    ITEM **item, const dsSTRID &key, const DBG_CINE_ACTOR_INFO &value)
{
    ITEM *newItem = reinterpret_cast<ITEM *>(
        this->allocator.GetEmpty());

    if (newItem) {
        newItem->next = nullptr;
        newItem->key.id = key.id;
        newItem->value.filePath.pBuffer = nullptr;
        dsTSTRING_BUF_HEADER<char> *valueBuffer = value.filePath.pBuffer;
        newItem->value.filePath.pBuffer = valueBuffer;
        ++valueBuffer->refCount;
        newItem->value.wasUpdatedCurFrame = value.wasUpdatedCurFrame;

        newItem->next = *item;
        *item = newItem;
    } else {
        // DEVIATION: decompile shows an unconditional `*(int*)0 = *item;` on the OOM path — a
        // deliberate null-deref crash, not a decompiler artifact (matches disasm at 0x823B9620).
        *reinterpret_cast<ITEM **>(0) = *item;
        *item = nullptr;
    }
}
