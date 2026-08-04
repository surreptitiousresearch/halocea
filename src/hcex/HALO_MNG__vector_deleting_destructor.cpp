#include "../headers/hcex/HALO_MNG.h"

// 0x823D26DC — HALO_MNG::`vector deleting destructor'(char a2). This is the standard MSVC
// compiler-synthesized thunk that exists automatically for any class with a virtual destructor
// (here, inherited from iaIACTOR): it runs ~HALO_MNG() [-> ~iaIACTOR()], then conditionally calls
// operator delete(this) when bit 0 of `a2` is set. There is no separate hand-written C++ source
// for it -- `delete pHaloMng;` through a HALO_MNG* (or iaIACTOR*) base pointer compiles to exactly
// this dispatch. Documented here (rather than fabricated as non-standard C++) to close out this
// method for the batch: its behavior is fully and correctly captured by ~HALO_MNG() plus the
// compiler's own delete-on-flag convention, both already reversed in HALO_MNG__dtor.cpp.
