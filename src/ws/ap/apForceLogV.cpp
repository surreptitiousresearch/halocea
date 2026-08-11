// apForceLogV @ 0x825C8D90 — ap_log.cpp
// Named-file logging primitive. `name` selects a log file by (absolute) path:
//   - string != null, not "//clr": format it with argPtr and append to that file (opening it,
//     creating a new _apLogList slot, on first use).
//   - string == null: close and free the slot for `name`.
//   - string == "//clr": rewind the file to the start (truncate-on-next-write semantics).
// Returns 1 on success, 0 on failure / empty name / message-too-large.
#include "../../headers/ws/ap/apLOG.h"
#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/os/OS_FILE.h"
#include "../../headers/ws/os/OS_FILE_SEEK.h"
#include <string.h>
#include <stdio.h>

// boundary — OS file layer (fio_boundary declares open/close/write; the rest are ap_log-local).
extern osFILE_HANDLE_DUMMY *osFileOpen(const char *name, int mode);
extern void osFileClose(osFILE_HANDLE_DUMMY *fp);
extern int  osFileWrite(osFILE_HANDLE_DUMMY *fp, const void *buf, int size);
extern void osFileSeek(osFILE_HANDLE_DUMMY *fp, int offset, OS_FILE_SEEK origin);
extern bool osFileIsReadOnly(const char *name);
extern void osFileSetNormal(const char *name);

// boundary — strong-assert stub and its context byte / suppression flag.
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
// ?Crash@STRONG_ASSERT_DUMMY@@QAAXPBD0H0@Z is a PUBLIC NON-STATIC member taking (const char*,
// const char*, int, const char*) after the implicit this — the trailing arg is the "info" string
// pointer. DEVIATION: this TU declared it as a namespace-scope free function with a leading
// context slot (and, before that, a `char msgCtx` tail, an artefact of empty_string having been
// declared here as a scalar `char`). Neither mangles to a symbol the image contains.
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

// Adjacent global marking the end of the _apLogList table (see apLOG.h).
extern "C" int gs_prefix; // referenced only for &gs_prefix as the scan bound

#define AP_LOG_NOT_FOUND 250 // slot-scan sentinel used by the shipped code

int apForceLogV(const char *name, char *string, char *argPtr)
{
    if (name[0] == '\0')
        return 0;

    char absPath[272];
    fnmBuildAbsolutePath(absPath, name);

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
            return 0; // table full

        if (osFileIsReadOnly(absPath))
            osFileSetNormal(absPath);

        stream = osFileOpen(absPath, OS_FILE_CREATE | OS_FILE_WRITE); // 0xA
        if (!stream)
            return 0;

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
            return 1;
        }
        stream = _apLogList[foundIdx].stream;
        if (!stream)
            return 0;
    }

    if (string)
    {
        if (strcmp(string, "//clr") == 0)
        {
            osFileSeek(stream, 0, OS_FILE_SEEK_SET);
            return 1;
        }

        char message[2096];
        if (vsnprintf(message, 0x800, string, argPtr) == -1)
        {
            if (!IGNORE_STRONG_ASSERT)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "!\"Trying to print too large message\"",
                    "D:\\Projects\\code\\common\\src.sys\\ap\\ap_log.cpp", 398, empty_string);
            return 0;
        }
        osFileWrite(stream, message, (int)strlen(message));
    }

    return 1;
}
