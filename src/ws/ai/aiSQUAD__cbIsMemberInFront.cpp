#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiSQUAD::cbIsMemberInFront(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832A99A0
// ?cbIsMemberInFront@aiSQUAD@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "IsMemberInFront" callback: unbox the member index (argv[0]) and front name (argv[1]),
// forward to implIsMemberInFront, box the returned bool into retVal. `caller` self-releases.
void aiSQUAD::cbIsMemberInFront(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > frontName(argv[1], nullptr);
    int memberIdx = 0;
    argv->GetValue<int>(memberIdx, DSD_CONV_RETRIEVE);

    bool result = implIsMemberInFront(memberIdx, frontName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
    // out + frontName destructors self-release at scope end.
}
