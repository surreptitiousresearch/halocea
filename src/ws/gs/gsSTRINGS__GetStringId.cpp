/* ?GetStringId@gsSTRINGS@@QAAHPBD@Z @0x826BA188 */
#include "../../headers/ws/gs/gsSTRINGS.h"

// _apLog (?_apLog@@YAXPBDZZ) — Saber varargs logging sink. boundary.
extern void _apLog(const char *format, ...);

// 0x826BA188 — FindRecord(idText); on a hit, return mapTextIdToIdx[record].idString; on a miss,
// log a warning and return 0.
//
// DEVIATION: the decompiler emitted 6 extra `int a3..a8` parameters (mirroring FindRecord's
// phantom-param artifact, since this function forwards them straight into the FindRecord call)
// plus 6 unused stack locals `v9..v14` that are passed through uninitialized — none of them are
// read or written apart from being forwarded. The real signature is (gsSTRINGS *this,
// const char *idText); see gsSTRINGS__FindRecord.cpp for the FindRecord-side confirmation.
int gsSTRINGS::GetStringId(const char *idText)
{
    int record = FindRecord(idText);
    if (record != -1)
        return mapTextIdToIdx[record].idString;

    _apLog("~Strings,Warning~GS_STRINGS: Can't find string \"%s\"", idText);
    return 0;
}
