#pragma once
#include "UI_FRAME.h"
#include "../ds/MAP.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../m3d/m2dV.h"
#include "../os/osLOCK.h"
#include "../../hcex/hcex_param_list_boundary.h" // dsVECTOR_PARAM_LIST
// ws-engine ui_new: the concrete UI_FRAME implementation manager (menus/HUD frames).
// DB-verified layout (DB type ui_new::UI_FRAME_MGR): UI_FRAME (base)@0 (84B),
// UI_MGR_IFACE (base)@84 (4B), frames@88 (ds::MAP<dsSTRID,FrameInfo,ds::HASH,ds::CMP,
// dsNODE_CACHE_ALLOCATOR>, 84B), activeFrameStack@172 (dsVECTOR<dsSTRID,8>), startFrame@192,
// frameOperations@196 (dsVECTOR<FrameOperation,8>), releaseQueue@216 (dsVECTOR<UI_FRAME*,8>),
// mousePosition@236 (m2dV), frameStackAccessLock@244 / frameOperationsLock@296 /
// releaseQueueLock@348 (osLOCK, 52B each) — size 400. Method bodies stay an extern boundary.

namespace ui_new {

struct UI_MGR_IFACE_vtbl; // boundary — slot table not expanded

// DB types_members ui_new::UI_MGR_IFACE: __vftable@0 — size 4.
typedef struct UI_MGR_IFACE {
    UI_MGR_IFACE_vtbl *__vftable; // 0x00
} UI_MGR_IFACE;

// DB types_members ui_new::UI_FRAME_MGR::FrameInfo — size 40. (DB nests it inside
// UI_FRAME_MGR; declared as an underscore-flattened tag + typedef here, same type.)
typedef struct ui_new_UI_FRAME_MGR_FrameInfo {
    UI_FRAME           *frame;      // 0x00
    int                 whenLoad;   // 0x04
    int                 whenUnload; // 0x08
    dsVECTOR_PARAM_LIST params;     // 0x0C
} ui_new_UI_FRAME_MGR_FrameInfo;
typedef ui_new_UI_FRAME_MGR_FrameInfo FrameInfo;

// DB types_enum_values ui_new::UI_FRAME_MGR::FrameOperationType.
enum ui_new_UI_FRAME_MGR_FrameOperationType : int {
    FSW_STACK_NOP       = 0,
    FSW_STACK_PUSH      = 1,
    FSW_STACK_PUSH_OVER = 2,
    FSW_STACK_POP       = 3,
    FSW_RESET           = 4,
    FSW_TERM            = 5,
};
typedef ui_new_UI_FRAME_MGR_FrameOperationType FrameOperationType;

// DB types_members ui_new::UI_FRAME_MGR::FrameOperation — size 12.
typedef struct ui_new_UI_FRAME_MGR_FrameOperation {
    FrameOperationType operType; // 0x00
    dsSTRID            frameID;  // 0x04
    int                plrId;    // 0x08
} ui_new_UI_FRAME_MGR_FrameOperation;
typedef ui_new_UI_FRAME_MGR_FrameOperation FrameOperation;

struct UI_FRAME_MGR : UI_FRAME {
    /* 0x54 (84) second base class (UI_MGR_IFACE, 4B) — spelled as a leading member because the
     * verifier's header parser handles only single inheritance; same layout. */
    UI_MGR_IFACE mgrIface;

    ds::MAP<dsSTRID, FrameInfo, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> frames; // 0x58
    dsVECTOR<dsSTRID, 8>        activeFrameStack;     // 0xAC
    dsSTRID                     startFrame;           // 0xC0
    dsVECTOR<FrameOperation, 8> frameOperations;      // 0xC4
    dsVECTOR<UI_FRAME *, 8>     releaseQueue;         // 0xD8
    m2dV                        mousePosition;        // 0xEC
    osLOCK                      frameStackAccessLock; // 0xF4
    osLOCK                      frameOperationsLock;  // 0x128
    osLOCK                      releaseQueueLock;     // 0x15C

    // 0x82780BC8-ish — devirtualized call seen at ui_new::UI_PROXY::ProcessRender's call site
    // (IDA renders it as a raw function-pointer-cast indirect call through the vtable; treated
    // here as the ordinary virtual method it is). Not reversed in this batch. boundary.
    void ProcessRender();
};

} // namespace ui_new
