#include "apDEFRAG_MNG.h"
#include "apDEFRAG_POOL.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"

// apDEFRAG_MNG::FindDesc @ 0x827B3C80 — const (ABA mangle).
// Scan every pool's descriptor list for the descriptor whose id string equals chunk->Id()
// (byte-wise compare); return it, or nullptr if none matches.
apDEFRAG_CHUNK_DESC *apDEFRAG_MNG::FindDesc(apDEFRAG_CHUNK *pChunk) const
{
    for (int poolIdx = 0; poolIdx < this->pools.nElem; ++poolIdx)
    {
        apDEFRAG_POOL *pool = this->pools[poolIdx];
        for (int descIdx = 0; descIdx < pool->descs.nElem; ++descIdx)
        {
            const char *descId  = pool->descs.pData[descIdx]->id.pBuffer->str;
            const char *chunkId = pChunk->__vftable->Id(pChunk);

            const unsigned char *d = (const unsigned char *)descId;
            const unsigned char *c = (const unsigned char *)chunkId;
            int diff;
            do
            {
                int descChar = *d;
                diff = descChar - *c;
                if (descChar == 0)
                    break;
                ++d;
                ++c;
            }
            while (diff == 0);

            if (diff == 0)
                return pool->descs[descIdx];
        }
    }
    return nullptr;
}
