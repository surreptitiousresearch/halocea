// gsSR_MANAGER::CopyDelayed2LastValid @ 0x82D27318
// Promote both int-managers' delayed collision queries to their last-valid snapshot, then:
//   1. Walk refineModifyVector back-to-front: drop entries whose weak cdtREFINE handle has died,
//      otherwise invoke the live refine's virtual Modify().
//   2. Fire every registered cdt-processed notifier, then clear the notifier list.
//
// Deviation from decompiler: the two int-manager sub-calls are shown against the phantom
// boundary `srMANAGER`/`srJobs.__cl.line` field names; the disasm (0x82D27330: this+0x20;
// 0x82D27338: this+0x38) confirms they are mngPPU (this+0x20) and mngSPU (this+0x38).

#include "../../headers/ws/gs/gsSR_MANAGER.h"
#include "../../headers/ws/gs/gsSR_CDT_PROCESSED_NOTIFIER.h"
#include "../../headers/ws/cdt/cdtREFINE.h"

void gsSR_MANAGER::CopyDelayed2LastValid()
{
    mngPPU.CopyDelayed2LastValid();
    mngSPU.CopyDelayed2LastValid();

    for (int i = refineModifyVector.nElem - 1; i >= 0; --i)
    {
        ds::WEAK_PTR<cdtREFINE> &weak = refineModifyVector[i];
        bool isDead = weak.pHandle ? (weak.pHandle->pPtr == nullptr) : true;
        if (isDead)
        {
            refineModifyVector.Erase(i, 1);
        }
        else
        {
            ds::WEAK_PTR<cdtREFINE> &live = refineModifyVector[i];
            cdtREFINE *pRefine = live.pHandle ? live.pHandle->pPtr : nullptr;
            pRefine->__vftable->Modify(pRefine);
        }
    }

    for (int j = 0; j < cdtProcessedNotifiers.nElem; ++j)
    {
        if (cdtProcessedNotifiers[j])
        {
            gsSR_CDT_PROCESSED_NOTIFIER *pNotifier = cdtProcessedNotifiers[j];
            pNotifier->__vftable->NotifyCdtProcessed(pNotifier);
        }
    }
    cdtProcessedNotifiers.nElem = 0;
}
