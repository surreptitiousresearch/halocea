#include "../headers/ws/vid/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"

// vidDEFRAG_CHUNK_OBJ_HCEX_X360::operator= @ 0x823F16E8
// Memberwise copy of every apDEFRAG_CHUNK base field plus this class's own `content` tag.
vidDEFRAG_CHUNK_OBJ_HCEX_X360 &vidDEFRAG_CHUNK_OBJ_HCEX_X360::operator=(const vidDEFRAG_CHUNK_OBJ_HCEX_X360 &that)
{
    this->pDesc = that.pDesc;
    this->pBuf = that.pBuf;
    this->ptr = that.ptr;
    this->size = that.size;
    this->ptrPrev = that.ptrPrev;
    this->state.state = that.state.state;
    this->content = that.content;
    return *this;
}
