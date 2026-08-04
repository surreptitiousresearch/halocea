#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // load_as_halo

// cbIsHaloMode @ 0x823D5278 — SSL callback "IsHaloMode() : bool". Returns whether the outer engine
// is currently running the legacy Blam (Halo) game, by boxing (load_as_halo != 0) into retVal.
extern "C" void cbIsHaloMode(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                             sslOBJ_REF caller)
{
    (void)argc;
    (void)argv;
    dsDATA result;
    result.type = nullptr;
    bool isHalo = (load_as_halo != 0);
    result.StoreValue<bool>(isHalo);
    retVal->StoreValue(result);
    // Local dsDATA `result` is torn down (type->Destroy) by its destructor on return.
}
