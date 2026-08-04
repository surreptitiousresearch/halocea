#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbCalcReticleDanger(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324B780
// ?cbCalcReticleDanger@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "CalcReticleDanger" getter callback: store implCalcReticleDanger's float result into `retVal`.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbCalcReticleDanger(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    // impl takes no args; the decompiler's forwarded (argc, argv) are dead callback registers.
    float danger = implCalcReticleDanger();
    retVal.StoreValue<float>(danger);
    // the caller ref releases itself at scope end.
}
