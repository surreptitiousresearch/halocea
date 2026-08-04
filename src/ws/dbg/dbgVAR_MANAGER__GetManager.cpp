#include "dbgVAR_MANAGER.h"

// dbgVAR_MANAGER::GetManager @ 0x82561210
// Process-wide singleton accessor. The binary's guard bit (`_S1_12` bit 0), the guarded
// placement-construct of the static object `m`, and the atexit-registered dynamic destructor for
// `m` are exactly what a function-local static (Meyers singleton) compiles to -- reconstructed
// idiomatically rather than hand-rolling the guard.
dbgVAR_MANAGER *dbgVAR_MANAGER::GetManager()
{
    static dbgVAR_MANAGER m;
    return &m;
}
