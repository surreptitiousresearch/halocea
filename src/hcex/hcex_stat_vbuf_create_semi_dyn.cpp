/* hcex_stat_vbuf_create_semi_dyn @0x823F09B0 — allocate a semi-dynamic static vertex buffer through the
 * ws-engine vertex/index buffer manager. Like hcex_stat_vbuf_create, but requests the SEMI_DYNAMIC defrag
 * state (apSTATE_T value 2) so the buffer can be re-uploaded without a full static rebuild. vidVBUF_MNG is a
 * ws-engine boundary; vidVBMng is its global instance. AllocStatData(content, state, size, desc) is a virtual
 * method (vtable slot at 0x38).
 *
 * DEVIATION: the decompiler rendered the by-value 1-byte apSTATE_T argument (2 == SEMI_DYNAMIC) as a bogus
 * pointer cast; verified against disasm it is passed by value in r5. */

#include "../headers/hcex/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"   /* VID_BC_VERT */

typedef struct vidVBUF_MNG   vidVBUF_MNG;
typedef struct vidDEFRAG_CHUNK vidDEFRAG_CHUNK;

#define VID_DEFRAG_STATE_SEMI_DYNAMIC 2

extern vidVBUF_MNG *vidVBMng;
extern vidDEFRAG_CHUNK *vidVBUF_MNG_AllocStatData(vidVBUF_MNG *self, int content_kind,
                                                   unsigned char state, int size, void *desc);

extern "C" vidDEFRAG_CHUNK *hcex_stat_vbuf_create_semi_dyn(int size)
{
    return vidVBUF_MNG_AllocStatData(vidVBMng, VID_BC_VERT, VID_DEFRAG_STATE_SEMI_DYNAMIC, size, 0);
}
