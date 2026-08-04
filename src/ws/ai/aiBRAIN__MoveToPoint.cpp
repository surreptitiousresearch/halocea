#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::MoveToPoint(const dsTSTRING<char>&, const dsTSTRING<char>&, const dsTSTRING<char>&, bool)
// @ 0x8324D3A8   ?MoveToPoint@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@00_N@Z
//
// Run the descriptor's "MoveToPoint" SSL function with (point, arrive, style, run); the four
// arguments are marshalled into a contiguous dsDATA[4] and the bool result is read back.
bool aiBRAIN::MoveToPoint(const dsTSTRING<char> &point, const dsTSTRING<char> &arrive,
                          const dsTSTRING<char> &style, bool run)
{
    dsDATA args[4];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(point);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(arrive);
    args[2].type = nullptr;
    args[2].StoreValue<dsTSTRING<char> >(style);
    args[3].type = nullptr;
    args[3].StoreValue<bool>(run);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnMoveToPoint(spDesc.pointee), 4, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
