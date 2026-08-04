#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSMART_PTR.h"
#include "../m3d/m2dV.h"
#include "../m3d/m3dRECT.h"
#include "../ssl/sslOBJ_REF.h"
struct dsREF_COUNT; // ws-engine ds refcount — used only as a pointer in spDesc's smart-ptr policy
// ws-engine ui_new: abstract base for a full-screen UI overlay/frame (loading screen, menus,
// etc). DB-verified layout (types_members ui_new::UI_FRAME) — 84 bytes, vtable pointer
// included. Its ctor/dtor/Enable are declared WITHOUT `virtual` (even though they are virtual
// in the real binary) so this spelling does not grow a second, compiler-synthesized vtable
// pointer on top of the explicit __vftable member below -- callers reach them only via
// explicit qualified calls (`ui_new::UI_FRAME::Enable(this)`), matching how the disassembly
// always invokes them.

struct dscDESC;    // boundary — dsc product descriptor
struct gsSSL_BASE; // boundary — ssl-scriptable ui object base

namespace ui_new {

struct UI_MGR_IFACE; // boundary — owning UI manager interface
struct UI_FRAME_vtbl; // boundary

typedef struct UI_FRAME {
    UI_FRAME_vtbl  *__vftable;      // 0x00 (a derived class overwrites it directly, matching
                                    //       the disasm idiom `this->__vftable = &Derived::`vftable'')
    dsSTRID         name;           // 0x04
    int             plrId;          // 0x08
    UI_MGR_IFACE   *mgr;            // 0x0C
    bool            isInited;       // 0x10
    bool            isEnabled;      // 0x11
    bool            isInputEnabled; // 0x12
    unsigned char   _pad13[1];      // 0x13 db-verified padding
    dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc; // 0x14
    dsTSTRING<char> ssl;            // 0x18
    sslOBJ_REF      sslObject;      // 0x1C
    m2dV            viewportSize;   // 0x20
    m3dRECT         viewportRC;     // 0x28
    unsigned int    viewportFlags;  // 0x38
    bool            viewportInitialized; // 0x3C
    unsigned char   _pad3D[3];      // 0x3D db-verified padding
    dsVECTOR<gsSSL_BASE *, 8> objects; // 0x40 (20B)

    UI_FRAME();    // boundary (body outside this re-source)
    ~UI_FRAME();   // boundary (body outside this re-source)
    void Enable(); // boundary (body outside this re-source)
    bool IsEnabled(); // 0x827D31AC — reads isEnabled@0x11 (see UI_FRAME__IsEnabled.cpp)
} UI_FRAME;                                  // 84 bytes

} // namespace ui_new
