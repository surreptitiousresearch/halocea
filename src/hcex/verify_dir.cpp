/* verify_dir @0x823C23D8 — ensure a directory exists, creating it if necessary. Returns 1 if the path
 * already exists on disk or was created; logs an HCEX error and returns 0 if creation fails. Used by the
 * HCEX checkpoint/save storage bootstrap.
 *
 * osFileIsExist / CreateDirectoryA are ws-engine / Win32 boundaries; dsTSTRING_flat is a ds boundary. */

#include "../headers/hcex/hcex_ds_boundary.h"   /* dsTSTRING_flat */

extern int  osFileIsExist(const char *path);
extern "C" int  CreateDirectoryA(const char *path, void *security_attributes);
extern void _apLog(const char *format, ...);

// DEVIATION: `extern "C"`. The binary exports this as the FLAT symbol `verify_dir` (@0x823C23D8);
// compiled as C++ without it the definition mangles to `?verify_dir@@YA...` and nothing links
// against it. 209 of the 219 flat-defining hcex TUs already do this -- these ten did not.
extern "C" int verify_dir(const dsTSTRING_flat *dir)
{
    if ( osFileIsExist(dir->pBuffer->str) || CreateDirectoryA(dir->pBuffer->str, 0) )
        return 1;

    _apLog("~HCEX,Error~verify_dir(): failed to create dir <%s>", dir->pBuffer->str);
    return 0;
}
