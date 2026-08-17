/* ?LODTerm@animINST@@QAAXXZ @0x82B0D660 */
#include "animINST.h"
#include "animLOD_LOD_INFO.h"
#include "anim_boundary.h"
#include <new>

// ?LODTerm@animINST@@QAAXXZ  (0x82B0D660) — tear down and free the level-of-detail table.
void animINST::LODTerm()
{
    animLOD *pLOD = this->pLOD;
    if (pLOD)
    {
        dlFree(pLOD->lodDist.pData);                     // free the LOD-distance vector storage
        pLOD->lods.~dsVECTOR<animLOD::LOD_INFO, 8>();     // destroy the per-level object vectors
        operator delete(pLOD);
    }
    this->pLOD = nullptr;
}
