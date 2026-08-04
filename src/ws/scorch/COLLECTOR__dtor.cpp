// scorch::COLLECTOR::~COLLECTOR @0x82C9153C — delete pOfl/pOflFace (when non-null), then free the
// five dsVECTOR members' backing storage directly (plain-array teardown, no per-element dtor
// walk — matches OBJ_INFO/unsigned short both being trivially destructible).
#include "../../headers/ws/scorch/COLLECTOR.h"

extern "C" void dlFree(void *ptr);

scorch::COLLECTOR::~COLLECTOR()
{
    if (pOfl) { pOfl->~cdtOFL(); operator delete(pOfl); }
    if (pOflFace) { pOflFace->~cdtOFL(); operator delete(pOflFace); }

    dlFree(faceIndListLM.pData);
    dlFree(faceIndListInst.pData);
    dlFree(faceIndList.pData);
    dlFree(objInfoListInst.pData);
    dlFree(objInfoList.pData);
}
