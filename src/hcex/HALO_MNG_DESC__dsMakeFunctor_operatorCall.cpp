#include "../headers/hcex/HALO_MNG_DESC.h"
#include "../headers/ws/ds/ds_new.h"

// 0x823E6ED0 — ??R?$dsMakeFunctor@VHALO_MNG_DESC@@$0A@@@QBAPAVHALO_MNG_DESC@@PBDH@Z
// dsMakeFunctor<HALO_MNG_DESC,0>::operator()(const char *file, unsigned line): the branded-new
// factory the dscMAKE_FAMILY machinery calls to construct a HALO_MNG_DESC on the tracked heap.
// Allocates 0xA4 bytes via the (size,file,line) placement operator new, base-constructs iaDESC
// and installs HALO_MNG_DESC's vtable — i.e. exactly `new(file,line) HALO_MNG_DESC()`.
//
// The functor object itself is stateless (dsMakeFunctor<T,0> carries no data), so `this` is
// unused; the mangled operator() is a const method returning the freshly built descriptor.
struct dsMakeFunctor_HALO_MNG_DESC {
    HALO_MNG_DESC *operator()(const char *file, unsigned int line) const;
};

HALO_MNG_DESC *dsMakeFunctor_HALO_MNG_DESC::operator()(const char *file, unsigned int line) const
{
    return new (file, line) HALO_MNG_DESC();
}
