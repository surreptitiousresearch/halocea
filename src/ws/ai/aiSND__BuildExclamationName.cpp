#include "../../headers/ws/ai/aiSND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// idasql: byte_8200155A — shared empty-string literal (CStr() fallback for an unset id).
extern char byte_8200155A[];
// _apLog @ engine logger. boundary.
extern void _apLog(const char *fmt, ...);

// aiSND::BuildExclamationName @ 0x83299828
// ?BuildExclamationName@aiSND@@UAA?AVdsSTRID@@ABV2@ABV?$dsTSTRING@D@@1@Z
//
// Compose the fully-resolved exclamation sound id: "<prefix><voiceType>_<name><postfix>", intern it
// (existing-only), and return it by value. An invalid result is logged as a BASS error. The
// intermediate dsTSTRINGs are copy-on-write copies, mirroring the original's buffer sharing.
dsSTRID aiSND::BuildExclamationName(const dsSTRID &name, const dsTSTRING<char> &prefix,
                                    const dsTSTRING<char> &postfix)
{
    const char *voice = voiceType.CStr();
    if (!voice)
        voice = byte_8200155A;

    dsTSTRING<char> built = prefix;
    built.Insert(built.pBuffer->strLen, voice, -1);
    built.Insert(built.pBuffer->strLen, "_", -1);

    const char *nm = name.CStr();
    if (!nm)
        nm = byte_8200155A;
    built.Insert(built.pBuffer->strLen, nm, -1);

    built.Insert(built.pBuffer->strLen, postfix.pBuffer->str, postfix.pBuffer->strLen);

    dsSTRID result(built, true);
    if (!result.IsValid())
        _apLog("~BASS,Error~%s: invalid exclamation name '%s'",
               pBrain->names.nameInst, built.pBuffer->str);

    return result;
}
