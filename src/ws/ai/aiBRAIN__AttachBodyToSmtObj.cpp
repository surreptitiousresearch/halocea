#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::AttachBodyToSmtObj(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x8324ECB8
// ?AttachBodyToSmtObj@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0@Z
//
// Run the descriptor's "AttachBodyToSmtObj" SSL function with (object, anchor), marshalled into a
// contiguous dsDATA[2]; returns its bool result.
bool aiBRAIN::AttachBodyToSmtObj(const dsTSTRING<char> &object, const dsTSTRING<char> &anchor)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(object);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(anchor);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnAttachBodyToSmtObj(spDesc.pointee), 2, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
