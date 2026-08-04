#pragma once
// ws-engine gs: observer notified once the spatial-refine (collision) manager has processed the
// current frame's deferred queries. DB-verified layout (types_members gsSR_CDT_PROCESSED_NOTIFIER,
// size 4): __vftable@0. vtable (types_members gsSR_CDT_PROCESSED_NOTIFIER_vtbl): dtr@0,
// NotifyCdtProcessed@4.

#include "../ds/dsVECTOR.h"
#include "../ds/WEAK_PTR.h"

struct cdtREFINE; // ../cdt/cdtREFINE.h — weak-referenced query filter (pointer/handle only here)

struct gsSR_CDT_PROCESSED_NOTIFIER {
    struct VTBL {
        void (*dtr)(gsSR_CDT_PROCESSED_NOTIFIER *self);                 // 0x00
        void (*NotifyCdtProcessed)(gsSR_CDT_PROCESSED_NOTIFIER *self);  // 0x04
    };
    VTBL *__vftable; // 0x00
};

// Module globals walked by gsSR_MANAGER::CopyDelayed2LastValid (0x82D27318):
//   refineModifyVector — pending cdtREFINE weak refs whose Modify() must run this frame; dead
//                        handles are Erase()d as encountered.
//   cdtProcessedNotifiers — observers fired after modification, then cleared.
extern dsVECTOR<ds::WEAK_PTR<cdtREFINE>, 8>       refineModifyVector;
extern dsVECTOR<gsSR_CDT_PROCESSED_NOTIFIER *, 8> cdtProcessedNotifiers;
