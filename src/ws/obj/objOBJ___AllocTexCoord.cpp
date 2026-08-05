#include <stdint.h>
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A81B08  ?_AllocTexCoord@objOBJ@@QAAHHH@Z
// (Re)allocate texture-coordinate channel `tcInd` (0..4). Element stride is 8 bytes, or 4 when the
// shared geometry flags the channel compressed (stateCompr bit 0x100<<tcInd). nVert==0 frees the
// channel. Returns 1 on success (or nVert<=0), 0 on allocation failure.
//
// CAVEAT: in the "channel was previously empty" path the binary memsets `stride*nVert - this->nVert`
// bytes (mixed units — verbatim from disasm at 0x82A81BEC, `subf r5,r11,r28`); reproduced exactly.

int objOBJ::_AllocTexCoord(unsigned int tcInd, int nVert)
{
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (!pGeom || tcInd > 4)
        return 0;

    if (nVert == 0) {
        _FreeTexCoord(tcInd);
        return 1;
    }

    int stride = 8;
    if ((pGeom->pSharedGeom->stateCompr.state & ((uint64_t)0x100 << tcInd)) != 0)
        stride = 4;

    bool wasEmpty = (GetTexCoordList(tcInd) == nullptr);
    void *oldList = GetTexCoordList(tcInd);
    char *newList = (char *)AllocObjData(tcInd + 15, stride * nVert, oldList);
    if (!newList && nVert > 0)
        return 0;

    int oldVertCount = this->nVert;
    if (wasEmpty) {
        memset(newList, 0, stride * nVert - oldVertCount); // verbatim mixed-unit count (see CAVEAT)
    } else if (nVert > oldVertCount) {
        memset(newList + oldVertCount * stride, 0, (nVert - oldVertCount) * stride); // zero grown tail
    }

    SetTexCoordList(tcInd, (m3dVTX *)newList);
    return 1;
}
