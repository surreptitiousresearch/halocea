/* ?AllocStatData@vidVBUF_MNG@@UAAPAUvidDEFRAG_CHUNK@@W4vidBUF_CONTENT@@V?$apSTATE_T@E@@HPAX@Z @0x827B9CE8 */
#include "../../headers/ws/vid/vidVBUF_MNG.h"
#include "../ds/dsCMP.h"

// 0x827B9CE8 -- ?AllocStatData@vidVBUF_MNG@@...
// Allocate a static (non-dynamic) GPU-buffer chunk of the requested content kind/size via the
// backing AllocStatDataImpl. On success, register the chunk in the sorted `chunks` vector
// (pointer-ordered, duplicates ignored) so it participates in defragmentation, and return it.
vidDEFRAG_CHUNK *vidVBUF_MNG::AllocStatData(vidBUF_CONTENT cont, apSTATE_T<unsigned char> *flags,
                                            int size, void *auxData)
{
    vidDEFRAG_CHUNK *chunk = this->AllocStatDataImpl(cont, flags, size, auxData);
    if (chunk)
    {
        apDEFRAG_CHUNK *entry = chunk; // vidDEFRAG_CHUNK derives apDEFRAG_CHUNK
        dsCMP cmp;
        // INS_DUP_IGNORE is nested in dsVECTOR<>::INS_SORT — qualify via the container's type.
        this->chunks.InsertSorted(entry, cmp, dsVECTOR<apDEFRAG_CHUNK *, 8>::INS_DUP_IGNORE);
    }
    return chunk;
}
