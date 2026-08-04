#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// aiBRAIN::implCallBhvFuncOnce @ 0x83296C38
// ?implCallBhvFuncOnce@aiBRAIN@@UAAXABV?$dsTSTRING@D@@0@Z
//
// Offer the SSL "On_<bhvPrefix>_<nameFunc>" ordered call to the mind's best-behaviour module; log a
// script error when no behaviour accepts it.
void aiBRAIN::implCallBhvFuncOnce(const dsTSTRING<char> &bhvName, const dsTSTRING<char> &funcName)
{
    if (!mind.best.pointee->AddBhvFuncCall(bhvName, funcName))
        _apLog("~AI,Ssl,Error~'%s': no bhv has accepted ordered call On_%s_%s",
               names.nameInst, bhvName.CStr(), funcName.CStr());
}
