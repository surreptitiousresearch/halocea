#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/hcex/hcex_start_cine_boundary.h" // hcex_cine_process_start, dsTSTRING_flat

// cbHcexStartCine @ 0x823B7118 — SSL callback "HcexStartCine(name : string = \"\")". Kick off
// cinematic capture parented to the named script (the first string argument, or none).
extern "C" void cbHcexStartCine(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                sslOBJ_REF caller)
{
    (void)retVal;
    if (argc >= 1)
    {
        // GetPtr yields the boxed string's dsTSTRING<char>* (layout-compatible with dsTSTRING_flat);
        // null when the argument slot is empty.
        const dsTSTRING_flat *parentScript =
            argv->type ? reinterpret_cast<const dsTSTRING_flat *>(argv->type->GetPtr(&argv->storage))
                       : nullptr;
        hcex_cine_process_start(parentScript);
    }
}
