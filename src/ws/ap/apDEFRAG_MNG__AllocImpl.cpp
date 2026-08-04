// apDEFRAG_MNG::AllocImpl @ 0x827B4FC8
// Find the descriptor for this chunk, bind it, record the size, and place the chunk
// into the descriptor's pool. Returns the chunk on success, nullptr if unmatched.
// Deviation: the decompiler typed the return as apDEFRAG_CHUNK_DESC*; the mangle
// (PAUapDEFRAG_CHUNK) says the return is apDEFRAG_CHUNK* — corrected here.
#include "apDEFRAG_MNG.h"

apDEFRAG_CHUNK *apDEFRAG_MNG::AllocImpl(apDEFRAG_CHUNK *chunk, int size)
{
    apDEFRAG_CHUNK_DESC *desc = this->FindDesc(chunk);
    if (desc)
    {
        chunk->pDesc = desc;
        chunk->size = size;
        desc->pool->__vftable->FitChunk(desc->pool, chunk);
        return chunk;
    }
    return nullptr;
}
