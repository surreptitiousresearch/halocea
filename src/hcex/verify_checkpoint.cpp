/* verify_checkpoint @0x823C5F98 — test whether a named checkpoint file exists for the running level and has
 * a readable header. Builds the on-disk path <pathToCheckpoints><curLevel>\ [coop\] <name>.<ext>, where the
 * "coop\" segment is inserted only in co-op, `name` is the leaf name of the requested checkpoint, and <ext>
 * is hcex_chpt_ext ("sav"). Returns 1 only if the file exists AND hcex_checkpoint_read_header succeeds;
 * otherwise 0 (including when no level is set or the checkpoint name is empty).
 *
 * The apPROCESSOR_LOCK checks are ws-engine debug thread-ownership assertions; dsTSTRING_flat / fnmGetNameStr /
 * haloENGINE_CONTROL are ws-engine boundaries.
 *
 * CAVEAT: the two-level C++ function-local-static lazy init the decompiler inlines for the shared empty
 * string (guard byte + dsTSTRING_flat::UnsafeInitEmpty's emptyStr static + its atexit destructor) is collapsed
 * here into the single ws-engine boundary call dsTSTRING_UnsafeInitEmpty(&full_path), preserving the
 * observable behaviour (full_path seeded with the shared empty ref-counted buffer, +1 ref). The co-op branch
 * releases its temporary via dsTSTRING_ReleaseBuffer (a `bl` in the disasm); the non-co-op branch inlines the
 * same release as an explicit refcount-decrement + dlFree — both reproduced as the compiler emitted them. */

#include "../headers/hcex/haloENGINE_CONTROL.h"   /* haloENGINE_CONTROL, dsTSTRING_flat (via hcex_ds_boundary) */

#define DS_STRING_HPP "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp"

typedef struct apPROCESSOR_LOCK apPROCESSOR_LOCK;
extern apPROCESSOR_LOCK dsTSTRING_processorLock;
extern void apPROCESSOR_LOCK_CheckCurrentProcessor(apPROCESSOR_LOCK *lock, const char *file, int line);

extern haloENGINE_CONTROL *haloEngineCtrl;
extern const dsTSTRING_flat *haloENGINE_CONTROL_GetCurLevel(haloENGINE_CONTROL *self);
extern const dsTSTRING_flat *haloENGINE_CONTROL_GetPathToCheckpoints(haloENGINE_CONTROL *self);
extern int  haloENGINE_CONTROL_IsCoop(haloENGINE_CONTROL *self);

/* dsTSTRING<char> operations (ws-engine boundary; free-function form of the C++ thiscall/operators) */
extern dsTSTRING_flat *fnmGetNameStr(dsTSTRING_flat *out, const dsTSTRING_flat *in);   /* leaf name of a path */
extern void       dsTSTRING_UnsafeInitEmpty(dsTSTRING_flat *out);            /* = shared empty, +1 ref */
extern dsTSTRING_flat *dsSPrintf(dsTSTRING_flat *result, const char *fmt, ...);
extern void       dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);                 /* operator= */
extern void       dsTSTRING_ReleaseBuffer(dsTSTRING_flat *self, dsTSTRING_BUF_HEADER<char> *buffer);
extern void       dsTSTRING_dtor(dsTSTRING_flat *s);                                           /* ~dsTSTRING_flat */

extern "C" char *hcex_chpt_ext;                              /* "sav" */
extern "C" int   hcex_checkpoint_read_header(const char *filename);
extern int   osFileIsExist(const char *path);
extern "C" void dlFree(void *ptr);

// DEVIATION: `extern "C"`. The binary exports this as the FLAT symbol `verify_checkpoint` (@0x823C5F98);
// compiled as C++ without it the definition mangles to `?verify_checkpoint@@YA...` and nothing links
// against it. 209 of the 219 flat-defining hcex TUs already do this -- these ten did not.
extern "C" int verify_checkpoint(const dsTSTRING_flat *checkpoint)
{
    const dsTSTRING_flat *cur_level = haloENGINE_CONTROL_GetCurLevel(haloEngineCtrl);
    if ( !cur_level->pBuffer->strLen || !checkpoint->pBuffer->strLen )
        return 0;

    dsTSTRING_flat name;
    fnmGetNameStr(&name, checkpoint);

    dsTSTRING_flat full_path;
    full_path.pBuffer = 0;
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 98);
    dsTSTRING_UnsafeInitEmpty(&full_path);

    if ( haloENGINE_CONTROL_IsCoop(haloEngineCtrl) )
    {
        const dsTSTRING_flat *level = haloENGINE_CONTROL_GetCurLevel(haloEngineCtrl);
        const dsTSTRING_flat *path_to_checkpoints = haloENGINE_CONTROL_GetPathToCheckpoints(haloEngineCtrl);

        dsTSTRING_flat assembled;
        dsTSTRING_flat *built = dsSPrintf(&assembled, "%s%s\\coop\\%s.%s",
                                     path_to_checkpoints->pBuffer->str, level->pBuffer->str,
                                     name.pBuffer->str, hcex_chpt_ext);
        dsTSTRING_assign(&full_path, built);
        dsTSTRING_ReleaseBuffer(&assembled, assembled.pBuffer);
    }
    else
    {
        const dsTSTRING_flat *level = haloENGINE_CONTROL_GetCurLevel(haloEngineCtrl);
        const dsTSTRING_flat *path_to_checkpoints = haloENGINE_CONTROL_GetPathToCheckpoints(haloEngineCtrl);

        dsTSTRING_flat assembled;
        dsTSTRING_flat *built = dsSPrintf(&assembled, "%s%s\\%s.%s",
                                     path_to_checkpoints->pBuffer->str, level->pBuffer->str,
                                     name.pBuffer->str, hcex_chpt_ext);
        dsTSTRING_assign(&full_path, built);

        /* inlined dsTSTRING_flat::ReleaseBuffer */
        apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 1014);
        if ( !--assembled.pBuffer->refCount )
            dlFree(assembled.pBuffer);
    }

    int exists_and_readable;
    if ( osFileIsExist(full_path.pBuffer->str) )
        exists_and_readable = hcex_checkpoint_read_header(full_path.pBuffer->str) ? 1 : 0;
    else
        exists_and_readable = 0;

    dsTSTRING_dtor(&full_path);
    dsTSTRING_dtor(&name);
    return exists_and_readable;
}
