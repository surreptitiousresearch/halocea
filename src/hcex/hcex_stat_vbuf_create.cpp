/* hcex_stat_vbuf_create @0x823F1B60 — allocate an HCEX static vertex buffer from the ws-engine defrag
 * manager and tag its content as vertex data. apDEFRAG_MNG is a ws-engine boundary type; Alloc is the
 * apDEFRAG_MNG::Alloc<vidDEFRAG_CHUNK_OBJ_HCEX_X360> template instantiation. */

#include "../headers/hcex/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"

typedef struct apDEFRAG_MNG apDEFRAG_MNG;

extern apDEFRAG_MNG *apDefragMng;
extern vidDEFRAG_CHUNK_OBJ_HCEX_X360 *apDEFRAG_MNG_Alloc_vidDEFRAG_CHUNK_OBJ_HCEX_X360(
    apDEFRAG_MNG *self, int size);

extern "C" vidDEFRAG_CHUNK_OBJ_HCEX_X360 *hcex_stat_vbuf_create(int size)
{
    vidDEFRAG_CHUNK_OBJ_HCEX_X360 *chunk =
        apDEFRAG_MNG_Alloc_vidDEFRAG_CHUNK_OBJ_HCEX_X360(apDefragMng, size);
    chunk->base.content = VID_BC_VERT;
    return chunk;
}
