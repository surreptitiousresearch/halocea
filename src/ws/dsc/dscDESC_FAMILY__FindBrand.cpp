#include "dscDESC_FAMILY.h"
#include "dscBRAND.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"

// Stateless name comparator functor (dsc subsystem) — see dsVECTOR<dscBRAND*,8>::FindSorted
// specialization (src/ws/ds/dsVECTOR_dscBRANDptr_8__FindSorted.cpp) for its inlined compare
// body against dscBRAND::name. Defined complete (not just forward-declared) here since a
// temporary instance is default-constructed to pass to FindSorted.
template<class T, int A, int B>
struct dsNAME_CMP {}; // boundary — empty comparator

// dscDESC_FAMILY::FindBrand(const dsTSTRING<char>&) const @ 0x827108E8
// Binary-search this family's name-ascending brandList for `nameBrand`; returns the matching
// dscBRAND, or null if not registered.
dscBRAND *dscDESC_FAMILY::FindBrand(const dsTSTRING<char> &nameBrand) const
{
    int index = this->brandList.FindSorted(nameBrand, dsNAME_CMP<dscBRAND, 0, 0>());
    if (index == -1)
        return nullptr;
    return this->brandList[index];
}
