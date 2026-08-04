#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiCOORD_GROUP::cbFilterWPSetByTargetHOPlane @ 0x832316F0
// ?cbFilterWPSetByTargetHOPlane@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: dispatch FilterWPSetByTargetHOPlane and store the bool result.
// DEVIATION: the decompiler passed (argc, argv) to the virtual; disasm (vtbl slot 0x14C, only r3
// set before bctrl) shows it takes no arguments.
void aiCOORD_GROUP::cbFilterWPSetByTargetHOPlane(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    bool result = this->FilterWPSetByTargetHOPlane();

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
