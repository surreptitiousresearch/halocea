#include "../headers/hcex/HALO_MNG_DESC.h"

// @0x823E6F38 — HALO_MNG_DESC::`vector deleting destructor'(char a2). Standard MSVC
// compiler-synthesized thunk for a class with a virtual destructor: runs ~HALO_MNG_DESC()
// (already reversed in HALO_MNG_DESC__dtor.cpp), then conditionally calls operator delete(this)
// when bit 0 of `a2` is set. No separate hand-written C++ source exists for it; `delete pDesc;`
// through a HALO_MNG_DESC* (or iaDESC*) base pointer compiles to exactly this dispatch.
