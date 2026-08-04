#include "../headers/ws/vid/vidDEFRAG_BUF_HCEX_X360.h"

// vidDEFRAG_BUF_HCEX_X360::vidDEFRAG_BUF_HCEX_X360 @ 0x823F164C
// Construct the apDEFRAG_BUF base (boundary — not reversed here), null pIBuf/pVBuf, then install
// this class's vtable.
vidDEFRAG_BUF_HCEX_X360::vidDEFRAG_BUF_HCEX_X360() : vidDEFRAG_BUF() // direct base is vidDEFRAG_BUF
{
    this->pIBuf = nullptr;
    this->__vftable = (apDEFRAG_BUF_vtbl *)&vidDEFRAG_BUF_HCEX_X360::vftable; // base __vftable type
    this->pVBuf = nullptr;
}
