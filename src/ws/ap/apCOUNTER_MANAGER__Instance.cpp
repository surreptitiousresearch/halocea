#include "apCOUNTER_MANAGER.h"

// Construct the manager (registers the console page etc.). boundary — not reversed here.
extern void apCOUNTER_MANAGER_construct(apCOUNTER_MANAGER *self); // ?apCOUNTER_MANAGER::apCOUNTER_MANAGER
// Compiler-generated dynamic atexit destructor for theSingleInstance. boundary.
extern void apCOUNTER_MANAGER_destroy_atexit(void);
extern "C" int atexit(void (*func)(void));

// apCOUNTER_MANAGER::Instance @ 0x82715EF0
// Meyers singleton: constructs `theSingleInstance` on first call (guarded by _S1_40) and
// registers its destructor with atexit. Reproduced as the standard local-static guard idiom.
apCOUNTER_MANAGER &apCOUNTER_MANAGER::Instance()
{
    static apCOUNTER_MANAGER theSingleInstance; // guarded by _S1_40
    static unsigned int      initGuard;

    if ((initGuard & 1) == 0)
    {
        initGuard |= 1u;
        apCOUNTER_MANAGER_construct(&theSingleInstance);
        atexit(&apCOUNTER_MANAGER_destroy_atexit); // dynamic atexit destructor for theSingleInstance
    }
    return theSingleInstance;
}
