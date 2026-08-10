/* hcex_set_current_checkpoint @0x823C77A8 — set the halo engine's current checkpoint from a checkpoint file
 * path/name. Strips the name+extension from the supplied path (fnmGetNameExtStr), validates it
 * (verify_checkpoint), and on success records it via haloENGINE_CONTROL::SetCurCheckpoint; on failure logs a
 * bad-checkpoint error. No-op for a NULL argument. The apPROCESSOR_LOCK checks are ws-engine debug
 * thread-ownership assertions; dsTSTRING_flat / haloENGINE_CONTROL are ws-engine boundaries. */

#include "../headers/hcex/haloENGINE_CONTROL.h"   /* haloENGINE_CONTROL, dsTSTRING_flat (via hcex_ds_boundary) */

typedef struct apPROCESSOR_LOCK apPROCESSOR_LOCK;
extern apPROCESSOR_LOCK dsTSTRING_processorLock;   /* dsTSTRING<char>::processorLock */
extern void apPROCESSOR_LOCK_CheckCurrentProcessor(apPROCESSOR_LOCK *lock, const char *file, int line);

extern haloENGINE_CONTROL *haloEngineCtrl;
extern void haloENGINE_CONTROL_SetCurCheckpoint(haloENGINE_CONTROL *self, const dsTSTRING_flat *name);
extern void fnmGetNameExtStr(_Out_ dsTSTRING_flat *out, const dsTSTRING_flat *path);   /* name + extension of a file path */
extern "C" int  verify_checkpoint(const dsTSTRING_flat *name);
extern void _apLog(const char *format, ...);
extern void dsTSTRING_dtor(dsTSTRING_flat *s);
extern void dsTSTRING_UnsafeInit(_Out_ dsTSTRING_flat *s, const char *src, int len, int flags); /* dsTSTRING<char>::UnsafeInit */
extern "C" void dlFree(void *ptr);

// DEVIATION: `extern "C"`. The binary exports this as the FLAT symbol `hcex_set_current_checkpoint` (@0x823C77A8);
// compiled as C++ without it the definition mangles to `?hcex_set_current_checkpoint@@YA...` and nothing links
// against it. 209 of the 219 flat-defining hcex TUs already do this -- these ten did not.
extern "C" void hcex_set_current_checkpoint(const char *checkpoint)
{
    if ( checkpoint )
    {
        dsTSTRING_flat path;
        path.pBuffer = 0;
        apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock,
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp", 149);
        dsTSTRING_UnsafeInit(&path, checkpoint, -1, 0);

        dsTSTRING_flat name;
        fnmGetNameExtStr(&name, &path);

        apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock,
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp", 1014);
        if ( --path.pBuffer->refCount == 0 )
            dlFree(path.pBuffer);

        if ( verify_checkpoint(&name) )
            haloENGINE_CONTROL_SetCurCheckpoint(haloEngineCtrl, &name);
        else
            _apLog("~HCEX,Error~couldnt set current checkpoint, bad checkpoint <%s>", (const char *)name.pBuffer);

        dsTSTRING_dtor(&name);
    }
}
