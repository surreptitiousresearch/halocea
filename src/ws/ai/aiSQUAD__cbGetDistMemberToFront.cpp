#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiSQUAD::cbGetDistMemberToFront(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832A97F0
// ?cbGetDistMemberToFront@aiSQUAD@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetDistMemberToFront" callback: unbox the member index (argv[0]) and front name (argv[1]),
// forward to implGetDistMemberToFront, box the returned float into retVal. `caller` self-releases.
void aiSQUAD::cbGetDistMemberToFront(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > frontName(argv[1], nullptr);
    int memberIdx = 0;
    argv->GetValue<int>(memberIdx, DSD_CONV_RETRIEVE);

    float dist = implGetDistMemberToFront(memberIdx, frontName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<float>(dist);
    retVal.StoreValue(out);
    // out + frontName destructors self-release at scope end.
}
