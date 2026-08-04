#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCOORD_GROUP::cbGetName @ 0x8322F670
// ?cbGetName@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: return the group's name string as the callback's return value.
void aiCOORD_GROUP::cbGetName(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    // GetName() returns a pointer to the group's stored name; the copy is ref-counted (the manual
    // buffer refCount inc/dec + dlFree in the binary is the inlined dsTSTRING copy-ctor/dtor).
    dsTSTRING<char> name = *GetName();

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<dsTSTRING<char> >(name);
    retVal.StoreValue(tmp);
}
