#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entCREATE_INFO.h"
#include "../../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../../headers/ws/ap/apCOUNTER_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_UNIT.h"

// ws-engine gs boundary global (same convention as msgADDR::Register).
extern unsigned long gsAppState; // ?gsAppState@@3KA — global app-state bitmask

// os atomic increment (?osLockedIncrement@@YAHPAH@Z). boundary.
extern int osLockedIncrement(int *pValue);

// ?gsSysLevel@@3PAVgsLVL_SYSTEM@@A — the single global gs-engine level-system message address.
extern gsLVL_SYSTEM *gsSysLevel;

// entENTITY::Register @ 0x82536068
//
// CAVEAT: the decompiler renders several boolean bit-tests through a `(_cntlzw(x) & 0x20) == 0`
// idiom (the standard PPC "convert flag word to bool" pattern the compiler emits for `!!x`-style
// tests). These are simplified below to plain `(x & mask) != 0` checks; verified equivalent
// against the raw disassembly bit-by-bit (cntlzw(0)=0x20 -> false branch, cntlzw(nonzero)<0x20 ->
// true branch). The `numEntityCreatesCnt` function-local static counter's compiler-generated
// init guard (`_S1_9`) and its `atexit`-registered destructor thunk are ABI boilerplate for a
// static local with a non-trivial constructor — reproduced here as an ordinary C++ function-local
// static, letting the compiler regenerate that boilerplate rather than modeling it explicitly.
bool entENTITY::Register(const entCREATE_INFO &info)
{
    if ((gsAppState & 0x2000) != 0)
        stateEnt.val |= 1;

    gsSysLevel->HandleAnimInstBeforeMSG_INIT(pInst);

    OnRegister(info);

    stateAddr.state |= 0x1000;

    if (!msgADDR::Register(info.pInitData))
        return false;

    gsSysLevel->HandleAnimInstAfterMSG_INIT(pInst);

    // stateAddr bit 0x8 (msgADDR "posted terminate" state) blocks registration once set.
    if ((stateAddr.state & 8) != 0)
        return false;

    // Per-call instrumentation counter ("other/entity_creates"), lazily constructed on first use.
    static apCOUNTER_CALL numEntityCreatesCnt("other/entity_creates");

    apCOUNTER_UNIT *unit = &numEntityCreatesCnt.counterUnits[0];

    // Bit 0x40 on the unit's state marks it "late-added"; apCOUNTER::GetNUnits treats such units
    // as implicitly carrying bit 0x02 too (see apCOUNTER.h). Here, a unit that is NOT late-added
    // has bit 0x02 forced on so the call below always counts it; a late-added unit only counts
    // when its own state already carries bit 0x02.
    unsigned char unitState = unit->state.state;
    if ((unitState & 0x40) == 0)
        unitState |= 2;

    if ((unitState & 2) != 0)
        osLockedIncrement(&numEntityCreatesCnt.callsTotal);

    return true;
}
