#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiSQUAD::GetDistMemberToFrontTarget(int, const dsTSTRING<char>&) @ 0x832A8A00
// ?GetDistMemberToFrontTarget@aiSQUAD@@QAAMHABV?$dsTSTRING@D@@@Z
// Run the descriptor's "GetDistMemberToFrontTarget" SSL function with (memberIdx, frontName) and
// return the float distance result.
float aiSQUAD::GetDistMemberToFrontTarget(int memberIdx, const dsTSTRING<char> &frontName)
{
    aiSQUAD_DESC *desc = reinterpret_cast<aiSQUAD_DESC *>(spDesc.pointee);

    dsDATA argv[2];

    dsDATA idxBox;
    idxBox.type = nullptr;
    idxBox.StoreValue<int>(memberIdx);
    argv[0].type = nullptr;
    argv[0].StoreValue(idxBox);

    dsTSTRING<char> frontCopy = frontName;
    dsDATA nameBox;
    nameBox.type = nullptr;
    nameBox.StoreValue<dsTSTRING<char> >(frontCopy);
    argv[1].type = nullptr;
    argv[1].StoreValue(nameBox);

    dsDATA retVal;
    retVal.type = nullptr;

    sslObject.CallFunc(desc->sslFuncId_GetDistMemberToFrontTarget, 2, argv, &retVal, sslOBJ_REF());

    float result = 0.0f;
    retVal.GetValue<float>(result, DSD_CONV_RETRIEVE);
    return result;
}
