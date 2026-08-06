// apCfgWriteInt @ 0x82659228 — write integer `num` into the global config under "section.key".
// Sets the current value first; if that key has no registered default it logs a warning and
// also writes it into the default store so the key exists. No-op when there is no global config.
//
// NOTE: the raw decompile carries a long tail of spurious int parameters (a4..a16) — these are
// stack/vararg slots the ABI reserved, not real arguments. a16 is merely the stack home used to
// pass `&num` by address to SetValue. The true signature is (section, key, num).
#include "../../headers/ws/ap/apCONFIG.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

extern "C" apCONFIG *gameConfig;                              // boundary — the global config database
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);   // boundary — ds formatted string build
extern "C" void dlFree(void *ptr);                        // boundary — dlmalloc free
extern void _apLog(const char *fmt, ...);                 // boundary — ap variadic log wrapper

static void releaseKeyString(dsTSTRING<char> &keyStr)
{
    if (--keyStr.pBuffer->refCount == 0)
        dlFree(keyStr.pBuffer);
}

void apCfgWriteInt(const char *section, const char *key, int num)
{
    if (!gameConfig)
        return;

    int value = num;

    dsTSTRING<char> keyStr = dsSPrintf("%s.%s", section, key);
    bool ok = gameConfig->SetValue<int>(keyStr.pBuffer->str, &value, 1);
    releaseKeyString(keyStr);

    if (!ok)
    {
        _apLog("~CFG,Warning~Changing key %s.%s in the global config: default value doesnt exist",
               section, key);
        dsTSTRING<char> defKeyStr = dsSPrintf("%s.%s", section, key);
        gameConfig->SetValue<int>(defKeyStr.pBuffer->str, &value, 0);
        releaseKeyString(defKeyStr);
    }
}
