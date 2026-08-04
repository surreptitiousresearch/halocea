// aiBASS::RegisterSslFuncCb @ 0x83242618
// ?RegisterSslFuncCb@aiBASS@@SA_NAAVsslCLASS_REF@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ssl/sslCB_HOST.h" // complete sslCB_HOST for the sslCB_MEMBER_FN cast target
#include "../../headers/ws/ssl/sslERROR.h"

// Register the three native script callbacks on `sslClass`. Returns false as soon as any
// registration reports an error, true once all three succeed.
bool aiBASS::RegisterSslFuncCb(sslCLASS_REF &sslClass)
{
    sslERROR err;

    err = sslClass.AddCbFunc(
        "PlayExclamationInstant(ai : object, sound : string) : bool",
        reinterpret_cast<sslCB_MEMBER_FN>(&aiBASS::cbPlayExclamationInstant), "", nullptr);
    if (err.id != SSL_ERR_NO_ERROR)
        return false;

    err = sslClass.AddCbFunc(
        "PlayExclamation(ai : object, sound : string, priority : float = -1, overlap : string = \"\") : bool",
        reinterpret_cast<sslCB_MEMBER_FN>(&aiBASS::cbPlayExclamation), "", nullptr);
    if (err.id != SSL_ERR_NO_ERROR)
        return false;

    err = sslClass.AddCbFunc(
        "GetCurrentEx(ai : object) : string",
        reinterpret_cast<sslCB_MEMBER_FN>(&aiBASS::cbGetCurrentEx), "", nullptr);
    if (err.id != SSL_ERR_NO_ERROR)
        return false;

    return true;
}
