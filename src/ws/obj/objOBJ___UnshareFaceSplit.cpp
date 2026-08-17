/* ?_UnshareFaceSplit@objOBJ@@QAAHXZ @0x82A80728 */
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "objSPLIT.h"
// 0x82A80728  ?_UnshareFaceSplit@objOBJ@@QAAHXZ
// If this object shares its face-split table (stateShare bit 0x4), give it a private copy:
// clear the shared flag, unshare the geometry, then duplicate the split. Returns 1 on success,
// 0 if geometry is missing or the duplication fails.

int objOBJ::_UnshareFaceSplit()
{
    if (!this->pGeom)
        return 0;

    if ((this->stateShare.state & 4) != 0) {
        _UnshareGeom();
        this->stateShare.state &= ~4; // clear "face split shared" flag

        objSPLIT *pSplit = this->pGeom->pSharedGeom->pSplit;
        if (pSplit) {
            objSPLIT *privateSplit = pSplit->Duplicate();
            if (!privateSplit)
                return 0;
            _UnshareGeom();
            this->pGeom->pSharedGeom->pSplit = privateSplit;
        }
    }
    return 1;
}
