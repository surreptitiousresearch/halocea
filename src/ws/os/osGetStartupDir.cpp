// osGetStartupDir @0x825E57E4 — copy the hard-coded startup drive/dir ("d:\\") into `dir`;
// STRONG_ASSERT that the result still fits under `length`.
#include <string.h>
#include "../../headers/ws/os/osLOCK.h" // pulls in the shared os-subsystem boundary decls

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h" // the only Crash is a C++ member, not a C symbol
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

void osGetStartupDir(char *dir, unsigned int length)
{
    strcpy(dir, "d:\\");
    if (!IGNORE_STRONG_ASSERT) {
        unsigned int len = (unsigned int)strlen(dir);
        if (len >= length)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("strlen(dir) < length", "D:\\Projects\\code\\common\\src.sys\\ap\\ap_os_xenon.cpp", 279, empty_string);
    }
}
