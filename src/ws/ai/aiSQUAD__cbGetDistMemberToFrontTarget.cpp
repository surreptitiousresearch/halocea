#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiSQUAD::cbGetDistMemberToFrontTarget(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832A98C8
// ?cbGetDistMemberToFrontTarget@aiSQUAD@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetDistMemberToFrontTarget" callback: unbox the member index (argv[0]) and front name
// (argv[1]), forward to implGetDistMemberToFrontTarget, box the returned float into retVal.
void aiSQUAD::cbGetDistMemberToFrontTarget(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > frontName(argv[1], nullptr);
    int memberIdx = 0;
    argv->GetValue<int>(memberIdx, DSD_CONV_RETRIEVE);

    float dist = implGetDistMemberToFrontTarget(memberIdx, frontName.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<float>(dist);
    retVal.StoreValue(out);
    // out + frontName destructors self-release at scope end.
}
