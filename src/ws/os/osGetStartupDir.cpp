// osGetStartupDir @0x825E57E4 — copy the hard-coded startup drive/dir ("d:\\") into `dir`;
// STRONG_ASSERT that the result still fits under `length`.
#include <string.h>
#include "../../headers/ws/os/osLOCK.h" // pulls in the shared os-subsystem boundary decls

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" void STRONG_ASSERT_DUMMY_Crash(void *self, const char *condition, const char *file, int line, const char *info); /* ?Crash@STRONG_ASSERT_DUMMY@@QAAXPBD0H0@Z @0x825202A8: r3=dead this, info=empty string (disasm) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

void osGetStartupDir(char *dir, unsigned int length)
{
    strcpy(dir, "d:\\");
    if (!IGNORE_STRONG_ASSERT) {
        unsigned int len = (unsigned int)strlen(dir);
        if (len >= length)
            STRONG_ASSERT_DUMMY_Crash(0, "strlen(dir) < length", "D:\\Projects\\code\\common\\src.sys\\ap\\ap_os_xenon.cpp", 279, empty_string);
    }
}
