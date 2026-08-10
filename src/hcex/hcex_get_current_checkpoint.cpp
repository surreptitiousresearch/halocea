/* hcex_get_current_checkpoint @0x823C7468 — build the on-disk path of the running game's current checkpoint
 * file and return it if the file exists. Returns NULL when no level or checkpoint is set, or when the
 * assembled path does not exist on disk. The path is <pathToCheckpoints><curLevel>\ [coop\] <curCheckpoint>,
 * where the "coop\" segment is inserted only in co-op. The returned pointer aliases a function-local static
 * dsTSTRING_flat that persists across calls; each call overwrites it.
 *
 * The apPROCESSOR_LOCK checks are ws-engine debug thread-ownership assertions; dsTSTRING_flat / haloENGINE_CONTROL
 * are ws-engine boundaries.
 *
 * CAVEAT: the two-level C++ function-local-static lazy init the decompiler inlines (the guard byte plus the
 * nested dsTSTRING_flat::UnsafeInitEmpty shared-empty-string static and their atexit destructors) is collapsed
 * here into a single ws-engine boundary call (dsTSTRING_UnsafeInitEmpty) guarded by the module init flag,
 * preserving the observable behaviour: on first entry the static path is seeded with the shared empty
 * ref-counted buffer and an atexit destructor is registered. */

#include "../headers/hcex/haloENGINE_CONTROL.h"   /* haloENGINE_CONTROL, dsTSTRING_flat (via hcex_ds_boundary) */

#define DS_STRING_HPP "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp"

typedef struct apPROCESSOR_LOCK apPROCESSOR_LOCK;
extern apPROCESSOR_LOCK dsTSTRING_processorLock;
extern void apPROCESSOR_LOCK_CheckCurrentProcessor(apPROCESSOR_LOCK *lock, const char *file, int line);

extern haloENGINE_CONTROL *haloEngineCtrl;
extern const dsTSTRING_flat *haloENGINE_CONTROL_GetCurLevel(haloENGINE_CONTROL *self);
extern const dsTSTRING_flat *haloENGINE_CONTROL_GetCurCheckpoint(haloENGINE_CONTROL *self);
extern const dsTSTRING_flat *haloENGINE_CONTROL_GetPathToCheckpoints(haloENGINE_CONTROL *self);
extern int  haloENGINE_CONTROL_IsCoop(haloENGINE_CONTROL *self);

/* dsTSTRING<char> operations (ws-engine boundary; free-function form of the C++ thiscall/operators) */
extern void dsTSTRING_UnsafeInitEmpty(_Out_ dsTSTRING_flat *out);                              /* = shared empty, +1 ref */
extern dsTSTRING_flat *dsTSTRING_concat(_Out_ dsTSTRING_flat *out, const dsTSTRING_flat *a, const dsTSTRING_flat *b);   /* operator+ */
extern dsTSTRING_flat *dsTSTRING_concat_cstr(_Out_ dsTSTRING_flat *out, const dsTSTRING_flat *a, const char *b);   /* operator+ */
extern void dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);                       /* operator= */
extern void dsTSTRING_Insert(dsTSTRING_flat *s, int at, const char *src, int len);
extern void dsTSTRING_InsertString(dsTSTRING_flat *s, int at, const dsTSTRING_flat *src);           /* Insert(at, dsTSTRING_flat) */

/* function-local-static lazy-init machinery (ws-engine / CRT boundary) */
extern unsigned int hcex_get_current_checkpoint_guard;      /* _S1_0 */
extern void hcex_get_current_checkpoint_result_dtor(void);
extern "C" int  atexit(void (*func)(void));
extern int  osFileIsExist(const char *path);
extern "C" void dlFree(void *ptr);

extern "C" char *hcex_get_current_checkpoint(void)
{
    static dsTSTRING_flat current_path;   /* returned buffer lives here across calls */

    if ( (hcex_get_current_checkpoint_guard & 1) == 0 )
    {
        hcex_get_current_checkpoint_guard |= 1u;
        current_path.pBuffer = 0;
        apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 98);
        dsTSTRING_UnsafeInitEmpty(&current_path);
        atexit(&hcex_get_current_checkpoint_result_dtor);
    }

    const dsTSTRING_flat *cur_level = haloENGINE_CONTROL_GetCurLevel(haloEngineCtrl);
    if ( !cur_level->pBuffer->strLen )
        return 0;

    const dsTSTRING_flat *cur_checkpoint = haloENGINE_CONTROL_GetCurCheckpoint(haloEngineCtrl);
    if ( !cur_checkpoint->pBuffer->strLen )
        return 0;

    const dsTSTRING_flat *level = haloENGINE_CONTROL_GetCurLevel(haloEngineCtrl);
    const dsTSTRING_flat *path_to_checkpoints = haloENGINE_CONTROL_GetPathToCheckpoints(haloEngineCtrl);

    dsTSTRING_flat path_plus_level;
    dsTSTRING_flat full_path;
    dsTSTRING_flat *joined = dsTSTRING_concat(&path_plus_level, path_to_checkpoints, level);
    dsTSTRING_flat *with_sep = dsTSTRING_concat_cstr(&full_path, joined, "\\");
    dsTSTRING_assign(&current_path, with_sep);

    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 1014);
    if ( !--full_path.pBuffer->refCount )
        dlFree(full_path.pBuffer);

    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 1014);
    if ( !--path_plus_level.pBuffer->refCount )
        dlFree(path_plus_level.pBuffer);

    if ( haloENGINE_CONTROL_IsCoop(haloEngineCtrl) )
        dsTSTRING_Insert(&current_path, current_path.pBuffer->strLen, "coop\\", -1);

    const dsTSTRING_flat *checkpoint = haloENGINE_CONTROL_GetCurCheckpoint(haloEngineCtrl);
    dsTSTRING_InsertString(&current_path, current_path.pBuffer->strLen, checkpoint);

    char *str = current_path.pBuffer->str;
    if ( osFileIsExist(str) )
        return str;
    return 0;
}
