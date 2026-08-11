#include "sslSYSTEM.h"
#include "../ds/dsTSTRING.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// @0x825CC270 — sslSYSTEM::Log (const char* base overload). Emit a line through the global sslLogCb
// sink of the form <indent><logPref><text><logPost>, preceded (once per accumulated batch) by a
// frame/timing banner. No-op when no sink is installed.
//
// CAVEAT: the banner's dsSPrintf varargs are shuffled by the PPC float/int register split in the
// decompile; the intended fields are (logTime, logFrames, logTimeTotal, logFramesTotal), reproduced
// here in that semantic order.
void sslSYSTEM::Log(const char *text)
{
    if (!sslLogCb)
        return;

    if (this->logFrames)
    {
        sslLogCb(0, "", 0); // blank separator line (raw empty_string empty-string constant)

        dsTSTRING<char> banner;
        sslLogCb(0,
                 dsSPrintf(&banner, "-  %.2f sec(s)  %i frame(s) passed  [ totals: %.2f / %i]-",
                           this->logTime, this->logFrames, this->logTimeTotal, this->logFramesTotal)
                     ->pBuffer->str,
                 0);

        this->logFrames = 0;
        this->logTime = 0.0f;
    }

    // Process-wide shared scratch line (lazily adopts the empty-string singleton on first use).
    static dsTSTRING<char> logLine;
    logLine.Clear();
    logLine.Insert(0, ' ', 2 * sslLogTabs);
    // The binary open-codes these string appends as an Insert(pos,len) gap + memcpy; equivalent to:
    logLine.Insert(logLine.pBuffer->strLen, this->logPref);
    logLine.Insert(logLine.pBuffer->strLen, text, -1);
    logLine.Insert(logLine.pBuffer->strLen, this->logPost);
    this->logPost.Clear();

    sslLogCb(0, logLine.pBuffer->str, 0);
}
