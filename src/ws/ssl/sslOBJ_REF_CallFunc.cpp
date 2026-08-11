// sslOBJ_REF_CallFunc @0x82523BE0
// Flattened bridges for the hcex boundary headers' two spellings of the dsSTRID-keyed
// sslOBJ_REF::CallFunc overload (0x82526578, re-sourced in sslOBJ_REF__CallFunc__3.cpp).
// Both binary call sites (hcex_kill_breakable_surface @0x823B21AC, hcex_obj_sync_game_state
// @0x823ED000/0x823ED030) dispatch by name with an sslOBJ_REF(nullptr)-adjacent caller ref;
// each wrapper reproduces its declaring header's exact signature so the C++ overload mangles
// match the extern decls.
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/ds_new.h"

// hcex_kill_breakable_surface_boundary.h spelling: `target` is the entity's embedded
// sslOBJ_REF (typed sslERROR* there as an opaque holder); the sret sslERROR is constructed
// into `result` (caller passes uninitialized storage, as the sret ABI does). `self` is the
// caller ref, consumed by value (a null-object ref at the one call site).
void sslOBJ_REF_CallFunc(sslERROR *result, sslERROR *target, dsSTRID *func,
                         int argc, void *args, void *err, sslOBJ_REF *self)
{
    new (result) sslERROR(reinterpret_cast<sslOBJ_REF *>(target)->CallFunc(
        *func, argc, (dsDATA *)args, (dsDATA *)err, *self));
}

// hcex_obj_sync_game_state_boundary.h spelling: fire-and-forget name-keyed call with no
// retVal and an empty caller ref (matches the binary sites: argc/argv/retVal all zero,
// caller = sslOBJ_REF(nullptr)); the returned sslERROR is released on scope exit.
void sslOBJ_REF_CallFunc(sslOBJ_REF *self, const dsSTRID *funcName, int nArgs, const dsDATA **args)
{
    sslERROR err = self->CallFunc(*funcName, nArgs, (dsDATA *)args, (dsDATA *)0,
                                  sslOBJ_REF((sslOBJECT *)0));
    (void)err;
}
