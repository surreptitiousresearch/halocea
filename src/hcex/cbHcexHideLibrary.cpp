#include "../headers/hcex/hcex_library_boundary.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ssl/sslERROR.h" // complete type — CallFunc returns sslERROR by value (dtor)

// cbHcexHideLibrary @ 0x823D3638 — SSL callback "HcexHideLibrary()". Hide the scripted "halo library"
// entity, fire its SSL hide function, and drop the script-camera override latch. No-op when the
// library entity was never created. Source: D:\Projects\code\HCEX\sources\halo_cine.cpp.
extern "C" void cbHcexHideLibrary(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                  sslOBJ_REF caller)
{
    (void)argc;
    (void)argv;
    (void)retVal;

    if (!hcexLibrary.pHandle || !hcexLibrary.pHandle->pPtr)
        return;

    hcexLibrary.pHandle->pPtr->Hide();

    // Re-read through the weak handle (Hide may have cleared it) before dispatching the SSL hide.
    entENTITY *library = hcexLibrary.pHandle ? hcexLibrary.pHandle->pPtr : nullptr;
    // Fire the SSL "library hide" function on the entity's script object; the returned sslERROR is
    // discarded (its refcount teardown is the destructor the decompiler inlines).
    library->sslObject.CallFunc(hcexLibraryHide, 0, nullptr, nullptr, sslOBJ_REF(nullptr));

    hcex_off_cam = 0;
}
