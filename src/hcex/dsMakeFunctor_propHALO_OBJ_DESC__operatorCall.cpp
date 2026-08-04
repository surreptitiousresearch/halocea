#include "../headers/ws/prop/propHALO_OBJ_DESC.h"
#include "../headers/ws/ds/ds_new.h"

// 0x823D0878 — ??R?$dsMakeFunctor@VpropHALO_OBJ_DESC@@$0A@@@QBAPAVpropHALO_OBJ_DESC@@PBDH@Z
// dsMakeFunctor<propHALO_OBJ_DESC,0>::operator()(const char *file, unsigned line): the branded-new
// factory the dscMAKE_FAMILY machinery calls to construct a propHALO_OBJ_DESC on the tracked heap.
// Allocates 0x60 (96) bytes via the (size,file,line) placement operator new, then base-constructs
// propENT_DESC and installs propHALO_OBJ_DESC's vtable — i.e. `new(file,line) propHALO_OBJ_DESC()`.
//
// The functor object itself is stateless (dsMakeFunctor<T,0> carries no data), so `this` is
// unused; the mangled operator() is a const method returning the freshly built descriptor.
struct dsMakeFunctor_propHALO_OBJ_DESC {
    propHALO_OBJ_DESC *operator()(const char *file, unsigned int line) const;
};

propHALO_OBJ_DESC *dsMakeFunctor_propHALO_OBJ_DESC::operator()(const char *file, unsigned int line) const
{
    return new (file, line) propHALO_OBJ_DESC();
}
