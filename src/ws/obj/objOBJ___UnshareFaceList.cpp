#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "objFACE.h"
// 0x82A805D0  ?_UnshareFaceList@objOBJ@@QAAHXZ
// If this object shares its face list (stateShare bit 0x1), give it a private copy: clear the
// flag, allocate a fresh face list of nFace faces, and copy the shared faces into it. Returns 1
// on success (including the already-private case), 0 on missing geometry or allocation failure.

int objOBJ::_UnshareFaceList()
{
    if (!this->pGeom)
        return 0;

    if ((this->stateShare.state & 1) != 0) {
        _UnshareGeom();
        this->stateShare.state &= ~1ll; // clear "face list shared"

        objFACE *oldFaces = this->pGeom->pSharedGeom->faceList;
        if (oldFaces) {
            _UnshareGeom();
            this->pGeom->pSharedGeom->faceList = nullptr;
            if (!_AllocFaceList(this->nFace))
                return 0;
            int faceSize = GetFaceSize();
            memcpy(this->pGeom->pSharedGeom->faceList, oldFaces, (size_t)faceSize * this->nFace);
        }
    }
    return 1;
}
