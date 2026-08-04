#pragma once
#include "../ia/iaIACTOR.h"
#include "../ds/dsVECTOR.h"
#include "../ds/VAL_LIST.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ds/Deleter.h"
#include "../cdt/voxGRID.h"
#include "scrVOX.h"
#include "scrBATCH_DYN.h"
#include "scrSCORCH_MARK_DYN.h"
#include "COLLECTOR.h"
#include "scrSCORCH_INI.h"

struct scrSCORCH_DESC; // scr subsystem — decal/scorch descriptor (pointer/smart-ptr only)
struct dsSTRID;        // ds subsystem — interned string id
// ws-engine scorch: the top-level scorch/decal manager. Owns the voxel grid of static scorch
// batches/marks, the dynamic scorch batch list, and the current poly-collector. Registered as an
// iaIACTOR so it ticks with the ws message system. DB-verified layout (types_members
// scrSCORCH_MANAGER) -- size 404.

typedef struct scrSCORCH_MANAGER : iaIACTOR {
    dsVECTOR<scrVOX, 8>                    voxelsToAdd;         // 0xC8 (200)
    dsVECTOR<int, 8>                       voxelsIdx;           // 0xDC
    dsVECTOR<scrVOX, 8>                    scrVoxels;           // 0xF0
    voxGRID                                voxGrid;             // 0x104 (48)
    ds::VAL_LIST<scrSCORCH_MARK_DYN>       dynScorchList;       // 0x134 (40)
    dsVECTOR<scrBATCH_DYN *, 8>            dynScorchBatches;    // 0x15C (20)
    dsSHARED_PTR<scorch::COLLECTOR, 0, Deleter<scorch::COLLECTOR> > pCollector; // 0x170 (8)
    int                                    lastFrameUpdated;    // 0x178
    int                                    nScorchStatToUnfarm; // 0x17C
    bool                                   isUnfarming;         // 0x180
    int                                    memUsedForStat;      // 0x184
    int                                    memUsedForDyn;       // 0x188
    bool                                   isNoBlood;           // 0x18C
    float                                  voxDistOff;          // 0x190

    // 0x827394E0 -- tear down every static/dynamic scorch batch and mark, reset the stat counters,
    // and install a fresh poly-collector. Reversed in scrSCORCH_MANAGER__RemoveAll.cpp.
    void RemoveAll();

    // 0x827274D0 -- request that all currently-farmed static scorch marks be un-farmed on the
    // next update (arms the unfarm threshold). Reversed in scrSCORCH_MANAGER__UnfarmAll.cpp.
    void UnfarmAll();

    // --- Members touched by Add()/FindAround() (reversed in this batch) -------------------------
    // 0x82739078 -- register a new scorch/decal from an INI + desc: validates input, collects the
    // affected polygons, and adds the resulting static + dynamic marks, then re-prepares render.
    void Add(const scrSCORCH_INI *iniIn,
             dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > *scrDescIn);

    // 0x82733080 -- gather scorch marks of `classID` whose footprint overlaps a sphere at `pos`,
    // remove those that are redundant with the incoming mark, and report whether the new mark is
    // already fully covered (true = skip adding). `overlappableOnly` restricts the scan to marks
    // whose descriptor allows overlap. Signature per DB mangle (dsSTRID by value, m3dV by ref).
    bool FindAround(dsSTRID classID, const m3dV &pos, float radius, float radNoAround,
                    bool overlappableOnly);

    // Support helpers used by the two methods above. boundary — bodies external to this batch.
    // 0x8272AD70
    bool IsValidInput(const scrSCORCH_INI &ini,
                      dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > scrDesc) const;
    // 0x82738DA0 — returns the (possibly substituted) desc actually used.
    dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >
        CollectPolygons(scrSCORCH_INI &ini,
                        dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > scrDesc,
                        int flag);
    // 0x827377D8
    void AddStatScorchmarks(const scrSCORCH_INI &ini,
                            dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > scrDesc,
                            bool sync);
    // 0x82738440
    int AddDynScorchmarks(const scrSCORCH_INI &ini,
                          dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > scrDesc);
    // 0x82736058
    void PrepareRender();
    // 0x82730B00 (public), 0x8272EDF8 (voxel), 0x82731788 (dynamic-list iterator)
    void RemoveScorchmark(const m3dV &pos, float radius);
    void RemoveScorchmark(scrVOX &vox, int markIdx);
    void RemoveScorchmark(ds::VAL_LIST<scrSCORCH_MARK_DYN>::ITERATOR it);
} scrSCORCH_MANAGER;
