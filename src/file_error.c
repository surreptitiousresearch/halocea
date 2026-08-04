/* file_error @0x837E66C0 — release-build stub: discards the last Win32 error and resets it to 0. Both
 * `function_name` and `file` are unused (confirmed via disasm 0x837E66C0-0x837E66DC — no error logging
 * remains in this build, only the GetLastError/SetLastError(0) clear pair). */

#include "headers/file_reference.h"

extern unsigned int GetLastError(void);
extern void SetLastError(unsigned int error_code);

void file_error(const char *function_name, const file_reference *file)
{
    GetLastError();
    SetLastError(0);
}
