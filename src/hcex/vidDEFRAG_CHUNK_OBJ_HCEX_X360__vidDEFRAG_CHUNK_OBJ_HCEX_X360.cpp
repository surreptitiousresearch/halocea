#include "../headers/ws/vid/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"

// vidDEFRAG_CHUNK_OBJ_HCEX_X360::vidDEFRAG_CHUNK_OBJ_HCEX_X360 @ 0x823F12D0
// Zero every apDEFRAG_CHUNK base field (pDesc/pBuf/ptr/size/ptrPrev/state) and install this
// class's vtable. `content` (vidDEFRAG_CHUNK's own field) is left uninitialized, matching the
// disasm exactly (no store to offset 0x1C).
vidDEFRAG_CHUNK_OBJ_HCEX_X360::vidDEFRAG_CHUNK_OBJ_HCEX_X360()
{
    this->pDesc = nullptr;
    this->pBuf = nullptr;
    this->ptr = nullptr;
    this->size = 0;
    this->ptrPrev = nullptr;
    this->state.state = 0;
    this->__vftable = (apDEFRAG_CHUNK_vtbl *)&vidDEFRAG_CHUNK_OBJ_HCEX_X360::vftable; // base __vftable type
}
