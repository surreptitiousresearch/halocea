// cdtREFINE::cdtREFINE @0x827139E8 — construct a collision-query refinement filter.
// DEVIATION (2026-08-07): this was reconstructed with THREE parameters, deriving layerMask as
// HIDWORD(stateObjUsrIncl). That was a Hex-Rays 32-bit register-pair misread. The mangle
// ??0cdtREFINE@@QAA@HKV?$apSTATE_T@_J@@PAVanimINST@@@Z is (int, unsigned long,
// apSTATE_T<__int64>, animINST *), and the twelve-instruction body maps one argument register to
// one member: r4 -> state@0x14, r5 -> layerMask@0x18, r6 (a whole 64-bit GPR, stored with `std`)
// -> stateObjUsrIncl@0x08, r7 -> pInstSelf@0x10. layerMask is its own argument; the folded model
// produced 0xC0 where hcex_create_decals_delayed passes 0.
#include <stdint.h>
#include "../../headers/ws/cdt/cdtREFINE.h"

// &cdtREFINE::`vftable' — emitted vtable symbol. boundary.
extern cdtREFINE_vtbl cdtREFINE__vftable;

cdtREFINE::cdtREFINE(int flags, unsigned long layerMask, apSTATE_T<int64_t> stateObjUsrIncl,
                     animINST *pInstSelf)
{
    weakSelf.pHandle = nullptr;                  // li r10, 0 ; stw r10, 4(r3)
    __vftable = &cdtREFINE__vftable;             // stw r9, 0(r3)
    this->stateObjUsrIncl = stateObjUsrIncl;     // std r6, 8(r3)
    this->pInstSelf = pInstSelf;                 // stw r7, 0x10(r3)
    state.state = flags;                         // stw r4, 0x14(r3)
    this->layerMask = layerMask;                 // stw r5, 0x18(r3)
    specMode = (SELDOM)5; // raw ctor writes 5, outside the named SELDOM range (0..3)
}
