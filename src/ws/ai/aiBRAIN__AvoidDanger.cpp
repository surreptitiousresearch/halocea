#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::AvoidDanger(const dsTSTRING<char>&) @ 0x83250CD8
// ?AvoidDanger@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "AvoidDanger" SSL function with danger name `danger` as its single argument
// and return the bool result.
bool aiBRAIN::AvoidDanger(const dsTSTRING<char> &danger)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(danger);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnAvoidDanger(spDesc.pointee), 1, &arg, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg / retVal destructors release their stored values at scope end.
}
