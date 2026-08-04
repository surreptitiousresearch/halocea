#include "../headers/ws/prop/propHALO_OBJ.h"
#include "../headers/ws/ds/ds_new.h"

// 0x823D0548 — ??R?$dsMakeFunctor@VpropHALO_OBJ@@$0A@@@QBAPAVpropHALO_OBJ@@PBDH@Z
// dsMakeFunctor<propHALO_OBJ,0>::operator()(const char *file, unsigned line): the branded-new
// factory the dscMAKE_FAMILY machinery calls to construct a propHALO_OBJ on the tracked heap.
// Allocates 0x4C (76) bytes via the (size,file,line) placement operator new, then base-constructs
// propENT and installs propHALO_OBJ's vtable — i.e. exactly `new(file,line) propHALO_OBJ()`.
//
// The functor object itself is stateless (dsMakeFunctor<T,0> carries no data), so `this` is
// unused; the mangled operator() is a const method returning the freshly built object.
struct dsMakeFunctor_propHALO_OBJ {
    propHALO_OBJ *operator()(const char *file, unsigned int line) const;
};

propHALO_OBJ *dsMakeFunctor_propHALO_OBJ::operator()(const char *file, unsigned int line) const
{
    return new (file, line) propHALO_OBJ();
}
