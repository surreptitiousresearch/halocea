#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::UseTurret(const dsTSTRING<char>&, const dsTSTRING<char>&, bool, bool) @ 0x8324EEC8
// ?UseTurret@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0_N1@Z
//
// Run the descriptor's "UseTurret" SSL function with (turret, action, instant, keep), marshalled into
// a contiguous dsDATA[4]; returns its bool result.
bool aiBRAIN::UseTurret(const dsTSTRING<char> &turret, const dsTSTRING<char> &action, bool instant, bool keep)
{
    dsDATA args[4];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(turret);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(action);
    args[2].type = nullptr;
    args[2].StoreValue<bool>(instant);
    args[3].type = nullptr;
    args[3].StoreValue<bool>(keep);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnUseTurret(spDesc.pointee), 4, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
