#pragma once
#include "../msg/msgADDR.h"
#include "../ds/dsEVENT_MGR.h"
#include "../ds/dsVECTOR.h"
#include "../ds/REF_TYPE.h"
#include "../inp/inpINPUT.h"
#include "../inp/inpKEY_PRESS_EVENT.h"
#include "gsiCONTEXT.h"
// ws-engine gs: the input-binding/dispatch system — owns the physical input device system
// (inpINPUT), the stack of active binding contexts, and "wait for key press" capture state.
// DB-verified layout (types_members gsINP_SYSTEM): msgADDR base@0 (32B), eventDispatcher@32,
// input@40, contextList@44, dbgCtx@64, generalCtx@68, onUpdateCB@72, keyPressEvent@76,
// isWaitForKeyPress@84.

// Per-frame update callback slot type (typedef'd so SetOnUpdateCB's declaration does
// not read as a function-pointer data member).
typedef void (*gsINP_UPDATE_CB)();

struct gsINP_SYSTEM : public msgADDR {
    // Nested dsEVENT_HANDLER subscriber that forwards ds-event-manager events back to the
    // owning gsINP_SYSTEM (DB-verified layout gsINP_SYSTEM::EVENT_DISP_gsINP_SYSTEM:
    // vftable@0, pObj@4 — size 8). Body is a boundary (outside this re-source).
    struct EVENT_DISP_gsINP_SYSTEM : public dsEVENT_HANDLER {
        gsINP_SYSTEM *pObj; // 0x04 owning input system

        void OnEvent(const dsEVENT *event); // boundary (outside this re-source)
    };

    EVENT_DISP_gsINP_SYSTEM              eventDispatcher; // 0x20 forwards ds events to this system
    inpINPUT                            *input;           // 0x28 physical device subsystem
    dsVECTOR<ds_data::REF_TYPE<gsiCONTEXT>, 8> contextList; // 0x2C active binding-context stack
    ds_data::REF_TYPE<gsiCONTEXT>        dbgCtx;           // 0x40 debug binding context handle
    ds_data::REF_TYPE<gsiCONTEXT>        generalCtx;       // 0x44 general/gameplay binding context handle
    void (*onUpdateCB)();                                  // 0x48 optional per-frame update callback
    inpKEY_PRESS_EVENT                   keyPressEvent;    // 0x4C last captured key-press for a pending wait
    bool                                  isWaitForKeyPress; // 0x54 true while capturing the next key press

    // 0x823C1360 — const accessor for the physical input device subsystem.
    inpINPUT *GetInput() const;

    // ?SetOnUpdateCB@gsINP_SYSTEM@@QAAXP6AXXZ@Z — install the per-frame input-update callback. boundary
    void SetOnUpdateCB(gsINP_UPDATE_CB cb);
    // ?GetDbgContext@gsINP_SYSTEM@@QAA?AV?$REF_TYPE@VgsiCONTEXT@@@ds_data@@XZ — this input system's
    // debug binding-context handle, returned by value (sret). boundary
    ds_data::REF_TYPE<gsiCONTEXT> GetDbgContext();
};
