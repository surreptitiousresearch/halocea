#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiSQUAD::IsMemberInFront(int, const dsTSTRING<char>&) @ 0x832A8BC8
// ?IsMemberInFront@aiSQUAD@@QAA_NHABV?$dsTSTRING@D@@@Z
// Run the descriptor's "IsMemberInFront" SSL function with (memberIdx, frontName) and return the
// boolean result.
bool aiSQUAD::IsMemberInFront(int memberIdx, const dsTSTRING<char> &frontName)
{
    aiSQUAD_DESC *desc = reinterpret_cast<aiSQUAD_DESC *>(spDesc.pointee);

    dsDATA argv[2];

    // arg0 = memberIdx (boxed int)
    dsDATA idxBox;
    idxBox.type = nullptr;
    idxBox.StoreValue<int>(memberIdx);
    argv[0].type = nullptr;
    argv[0].StoreValue(idxBox);

    // arg1 = frontName (boxed string; refcount-bumped copy)
    dsTSTRING<char> frontCopy = frontName;
    dsDATA nameBox;
    nameBox.type = nullptr;
    nameBox.StoreValue<dsTSTRING<char> >(frontCopy);
    argv[1].type = nullptr;
    argv[1].StoreValue(nameBox);

    dsDATA retVal;
    retVal.type = nullptr;

    sslObject.CallFunc(desc->sslFuncId_IsMemberInFront, 2, argv, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // idxBox/nameBox/frontCopy/argv[]/retVal destructors release their values at scope end.
}
