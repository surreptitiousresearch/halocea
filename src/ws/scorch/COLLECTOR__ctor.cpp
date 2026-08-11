// scorch::COLLECTOR::COLLECTOR @0x82C913C0 — default-construct scorchName/idNull("null")/cdtInfo,
// zero the five dsVECTOR members, set IsApplyTranspOnly=isShadeOnlySourceObj=false,
// isFilterByDiscontinuity=isFilterByNormal=true, null pObjSource/pAtkDisp, then heap-allocate
// pOfl (cdtOFL(500,0), single-level) and pOflFace (cdtOFL(1,3000)).
#include "../../headers/ws/scorch/COLLECTOR.h"

scorch::COLLECTOR::COLLECTOR() : scorchName(), idNull("null", true), cdtInfo()
{
    objInfoList.pData = nullptr;      objInfoList.nElem = 0;      objInfoList.allocated = 0;
    objInfoListInst.pData = nullptr;  objInfoListInst.nElem = 0;  objInfoListInst.allocated = 0;
    faceIndList.pData = nullptr;      faceIndList.nElem = 0;      faceIndList.allocated = 0;
    faceIndListInst.pData = nullptr;  faceIndListInst.nElem = 0;  faceIndListInst.allocated = 0;
    faceIndListLM.pData = nullptr;    faceIndListLM.nElem = 0;    faceIndListLM.allocated = 0;

    IsApplyTranspOnly = false;
    isShadeOnlySourceObj = false;
    isFilterByDiscontinuity = true;
    isFilterByNormal = true;
    pObjSource = nullptr;
    pAtkDisp = nullptr;

    pOfl = new cdtOFL(500, 0);
    pOflFace = new cdtOFL(1, 3000);
}
