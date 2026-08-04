// hcexBREAKABLE_SURF — one entry in the hcexBreakableSurfaces sorted vector: a (bsp,idx) key, a
// live scene instance, and the create-data to rebuild it. Boilerplate: default/copy ctor, dtor,
// operator=, scalar deleting dtor, operator< (the sort predicate dsVECTOR<hcexBREAKABLE_SURF,8>::
// FindSorted/InsertSorted binary-searches with). See src/headers/hcex/hcex_breakable_surf.h for the
// DB-verified layout.

#include "../../headers/hcex/hcex_breakable_surf.h"

// 0x823B16F8 — default-construct: default-construct the embedded scnINST_CREATE_DATA. bsp/idx/
// pInst are left uninitialized, matching the disassembly.
hcexBREAKABLE_SURF::hcexBREAKABLE_SURF()
{
}

// 0x823B1EBC — copy-construct: copy bsp/idx/pInst, then copy-construct cd from that.cd.
hcexBREAKABLE_SURF::hcexBREAKABLE_SURF(const hcexBREAKABLE_SURF &that)
    : bsp(that.bsp)
    , idx(that.idx)
    , pInst(that.pInst)
    , cd(that.cd)
{
}

// 0x823B1144 — destructor body is empty: `cd`'s destruction is the implicit per-member teardown
// (scnINST_CREATE_DATA::~scnINST_CREATE_DATA(), declared boundary in scnINST_CREATE_DATA.h). The
// disasm shows this as a visible CALL (the callee isn't trivial-sized) -- exactly the automatic
// member destruction the compiler emits for a destructor with no other explicit work.
hcexBREAKABLE_SURF::~hcexBREAKABLE_SURF()
{
}

// 0x823B137C — copy-assign bsp/idx/pInst, then copy-assign cd from that.cd.
hcexBREAKABLE_SURF &hcexBREAKABLE_SURF::operator=(const hcexBREAKABLE_SURF &that)
{
    bsp = that.bsp;
    idx = that.idx;
    pInst = that.pInst;
    cd = that.cd;
    return *this;
}

// 0x823B0900 — ascending order on the (bsp, idx) key: bsp first, then idx on a bsp tie.
// DEVIATION: the disassembly computes this via a branchless bit trick (an XOR-sign-bit /
// unsigned-compare combination) rather than a plain relational operator; verified equivalent to a
// straightforward signed less-than on whichever field is compared (bsp, or idx on a bsp tie).
bool hcexBREAKABLE_SURF::operator<(const hcexBREAKABLE_SURF &other) const
{
    if (bsp != other.bsp)
        return bsp < other.bsp;
    return idx < other.idx;
}

// 0x823B1320 (`??_G` mangle) — scalar deleting destructor: run ~hcexBREAKABLE_SURF(), then
// conditionally operator-delete `this` when bit 0 of deleteFlags is set.
hcexBREAKABLE_SURF *hcexBREAKABLE_SURF::ScalarDeletingDtor(unsigned char deleteFlags)
{
    this->~hcexBREAKABLE_SURF();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
