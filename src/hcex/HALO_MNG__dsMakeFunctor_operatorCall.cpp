#include "../headers/hcex/HALO_MNG.h"
#include "../headers/ws/ds/ds_new.h"

// @0x823D2608 — ??R?$dsMakeFunctor@VHALO_MNG@@$0A@@@QBAPAVHALO_MNG@@PBDH@Z
// dsMakeFunctor<HALO_MNG,0>::operator()(const char *file, unsigned line): the branded-new factory
// the dscMAKE_FAMILY machinery calls to construct a HALO_MNG on the tracked heap. Allocates 0xC8
// (200) bytes via the (size,file,line) placement operator new, base-constructs iaIACTOR and
// installs HALO_MNG's vtable — i.e. exactly `new(file,line) HALO_MNG()`.
//
// The functor object itself is stateless (dsMakeFunctor<T,0> carries no data), so `this` is
// unused; the mangled operator() is a const method returning the freshly built actor.
struct dsMakeFunctor_HALO_MNG {
    HALO_MNG *operator()(const char *file, unsigned int line) const;
};

HALO_MNG *dsMakeFunctor_HALO_MNG::operator()(const char *file, unsigned int line) const
{
    return new (file, line) HALO_MNG();
}
