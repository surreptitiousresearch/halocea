#include "MAP.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Clear @ 0x8255B888
// Return every live node in every bucket to the free list, then reset every bucket head to null
// and `count` to 0. Bucket capacity (`data.nElem`) is left unchanged — the table keeps its buckets
// for reuse rather than shrinking.
// DEVIATION/CAVEAT: the decompile's second loop (`for(k=0;k<nElem;++k) ;`) shows an EMPTY body,
// which would leave stale (already-recycled) node pointers sitting in the bucket array — breaking
// the very next Find/Insert's `if (data[bucket])` liveness check. Reconstructed as the obviously-
// required `data[k] = nullptr`, matching every sibling MAP Clear's contract; the empty body is
// treated as a decompiler store-elision artifact, not intended behavior.
template<>
void ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Clear()
{
    for (int i = 0; i < data.nElem; ++i) {
        if (data[i]) {
            ITEM *node = data[i];
            while (node) {
                ITEM *next = node->next;
                allocator.Delete(reinterpret_cast<dsMsgAddrMapDUMMY *>(node));
                node = next;
            }
        }
    }

    for (int k = 0; k < data.nElem; ++k)
        data[k] = nullptr; // CAVEAT: decompile showed an empty loop body here — see file header note

    count = 0;
}
