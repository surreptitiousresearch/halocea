#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::AvoidGrenade(int, const dsTSTRING<char>&, float) @ 0x83250E50
// ?AvoidGrenade@aiBRAIN@@QAA_NHABV?$dsTSTRING@D@@M@Z
//
// Run the descriptor's "AvoidGrenade" SSL function with (grenadeId, danger, radius) as its three
// arguments and return the bool result. The three arg words are laid down as a contiguous dsDATA
// array, matching the disassembly's argv pointer.
bool aiBRAIN::AvoidGrenade(int grenadeId, const dsTSTRING<char> &danger, float radius)
{
    dsDATA args[3];
    args[0].type = nullptr;
    args[0].StoreValue<int>(grenadeId);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(danger);
    args[2].type = nullptr;
    args[2].StoreValue<float>(radius);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnAvoidGrenade(spDesc.pointee), 3, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args / retVal destructors release their stored values at scope end.
}
