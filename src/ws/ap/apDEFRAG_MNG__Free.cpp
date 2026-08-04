// apDEFRAG_MNG::Free @ 0x827B2548
// Unlink the chunk from its backing buffer, then run its (scalar-deleting) destructor.
#include "apDEFRAG_MNG.h"
#include "apDEFRAG_BUF.h"

void apDEFRAG_MNG::Free(apDEFRAG_CHUNK *chunk)
{
    apDEFRAG_BUF *buffer = chunk->pBuf;
    buffer->__vftable->RemoveChunk(buffer, chunk);
    // Scalar-deleting dtr: deleteFlag=1 => run ~apDEFRAG_CHUNK then free (vtbl slot models the flag).
    chunk->__vftable->dtr_apDEFRAG_CHUNK(chunk, 1);
}
