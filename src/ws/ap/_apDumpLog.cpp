// _apDumpLog @0x825C9080 — ap_log.cpp
// Named-file dump primitive, structurally identical to apForceLogV but gated by the dump-enable
// state: it only does anything when a dump is being forced (dumpRightNow) or when append-logging
// is enabled (dumpLogWrite) AND the `writeLog` debug variable is set. `name` selects a log file
// by (absolute) path; `string` semantics match apForceLogV (append / null=close / "//clr"=rewind).
// Returns 1 on success, 0 on failure / disabled / empty-name / message-too-large.
#include "../../headers/ws/ap/apLOG.h"
#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/os/OS_FILE.h"
#include "../../headers/ws/os/OS_FILE_SEEK.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// boundary — OS file layer (fio_boundary declares open/close/write; the rest are ap_log-local).
extern osFILE_HANDLE_DUMMY *osFileOpen(const char *name, int mode);
extern void osFileClose(osFILE_HANDLE_DUMMY *fp);
extern int  osFileWrite(osFILE_HANDLE_DUMMY *fp, const void *buf, int size);
extern void osFileSeek(osFILE_HANDLE_DUMMY *fp, int offset, OS_FILE_SEEK origin);
extern bool osFileIsReadOnly(const char *name);
extern void osFileSetNormal(const char *name);

// boundary — dump-enable state and the `writeLog` debug variable.
extern "C" bool dumpRightNow;                  // 0x842090D0 — force-flush flag
extern "C" bool dumpLogWrite;                  // ap-log append-enable flag
extern "C" dbgVAR_SIMPLE<bool, 1> dbg_writeLog; // ap-log `writeLog` debug var

// boundary — strong-assert stub and its context byte / suppression flag.
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
namespace STRONG_ASSERT_DUMMY {
    // DEVIATION: slot 5 was `char msgCtx`, an artefact of empty_string having been declared here
    // as a scalar `char`. ?Crash@STRONG_ASSERT_DUMMY@@QAAXPBD0H0@Z is (const char*, const char*,
    // int, const char*) after the implicit this — the trailing arg is the "info" string pointer.
    void Crash(void *ctx, const char *expr, const char *file, int line, const char *info);
}

// Adjacent global marking the end of the _apLogList table (see apLOG.h).
extern "C" int gs_prefix; // referenced only for &gs_prefix as the scan bound

#define AP_LOG_NOT_FOUND 250 // slot-scan sentinel used by the shipped code

int _apDumpLog(const char *name, char *string, ...)
{
    va_list va;
    va_start(va, string);

    char absPath[272];
    fnmBuildAbsolutePath(absPath, name);

    if (!dumpRightNow && (!dumpLogWrite || !dbg_writeLog.value))
    {
        va_end(va);
        return 0;
    }
    if (absPath[0] == '\0')
    {
        va_end(va);
        return 0;
    }

    // locate an existing slot whose path matches absPath
    int foundIdx = 0;
    for (apLOG *slot = _apLogList; slot < (apLOG *)&gs_prefix; ++slot, ++foundIdx)
    {
        if (strcmp(slot->name, absPath) == 0)
            break;
    }

    osFILE_HANDLE_DUMMY *stream;

    if (foundIdx == AP_LOG_NOT_FOUND)
    {
        // no existing slot -> find a vacant one (empty name) and open the file into it
        int freeIdx = 0;
        for (apLOG *slot = _apLogList; slot < (apLOG *)&gs_prefix; ++slot, ++freeIdx)
        {
            if (slot->name[0] == '\0')
                break;
        }
        if (freeIdx == AP_LOG_NOT_FOUND)
        {
            va_end(va);
            return 0; // table full
        }

        if (osFileIsReadOnly(absPath))
            osFileSetNormal(absPath);

        stream = osFileOpen(absPath, OS_FILE_MAKEDIR | OS_FILE_CREATE | OS_FILE_WRITE); // 0x2A
        if (!stream)
        {
            va_end(va);
            return 0;
        }

        strcpy(_apLogList[freeIdx].name, absPath);
        _apLogList[freeIdx].stream = stream;
    }
    else
    {
        if (!string)
        {
            // close request: drop the slot
            osFileClose(_apLogList[foundIdx].stream);
            _apLogList[foundIdx].name[0] = '\0';
            va_end(va);
            return 1;
        }
        stream = _apLogList[foundIdx].stream;
        if (!stream)
        {
            va_end(va);
            return 0;
        }
    }

    if (string)
    {
        if (strcmp(string, "//clr") == 0)
        {
            osFileSeek(stream, 0, OS_FILE_SEEK_SET);
            va_end(va);
            return 1;
        }

        char message[2080];
        if (vsnprintf(message, 0x800, string, va) == -1)
        {
            if (!IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(
                    nullptr, "!\"Trying to print too large message\"",
                    "D:\\Projects\\code\\common\\src.sys\\ap\\ap_log.cpp", 317, empty_string);
            va_end(va);
            return 0;
        }
        osFileWrite(stream, message, (int)strlen(message));
    }

    va_end(va);
    return 1;
}
