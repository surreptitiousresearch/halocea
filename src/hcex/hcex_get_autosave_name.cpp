/* hcex_get_autosave_name @0x823C7A48 — build the on-disk path of the autosave checkpoint file for a given
 * level and return it, creating the directory tree as needed. The path is
 *   <pathToCheckpoints><levelName>[\coop]\<chptName>.<chptExt>
 * where the "\coop" segment is inserted (and its directory verified/created) only in co-op. Each directory
 * along the way is validated via verify_dir; if any cannot be created the function returns null. On the way it
 * also records "_autosave" into the "0HCEX/checkpoints/name" debug string variable. A null levelPath logs an
 * HCEX error and returns the previous (possibly empty) path string. The returned pointer aliases a
 * function-local static dsTSTRING_flat that persists across calls; each successful call overwrites it.
 *
 * dsTSTRING_flat / haloENGINE_CONTROL / dbgVAR_MANAGER / apPROCESSOR_LOCK are ws-engine (Saber) boundaries;
 * verify_dir is an HCEX sibling; fnmGetNameStr is a ws-engine filename helper.
 *
 * CAVEAT: the two-level C++ function-local-static lazy init the decompiler inlines (the guard byte plus the
 * nested dsTSTRING_flat::UnsafeInitEmpty shared-empty-string static and their atexit destructors) is collapsed
 * here into a single ws-engine boundary call (dsTSTRING_UnsafeInitEmpty) guarded by the module init flag,
 * preserving the observable behaviour (mirrors hcex_get_current_checkpoint). */

#include "../headers/hcex/haloENGINE_CONTROL.h"      /* haloENGINE_CONTROL, dsTSTRING_flat, dsTSTRING_UnsafeInit, dsSPrintf, dlFree */
#include "../headers/hcex/hcex_dbgvar_boundary.h"    /* dbgVAR_MANAGER, dbgVAR_STRING, dbg_hcex_ChptName */

#define DS_STRING_HPP "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp"

typedef struct apPROCESSOR_LOCK apPROCESSOR_LOCK;
extern apPROCESSOR_LOCK dsTSTRING_processorLock;
extern void apPROCESSOR_LOCK_CheckCurrentProcessor(apPROCESSOR_LOCK *lock, const char *file, int line);

extern haloENGINE_CONTROL *haloEngineCtrl;
extern const dsTSTRING_flat *haloENGINE_CONTROL_GetPathToCheckpoints(haloENGINE_CONTROL *self);
extern int  haloENGINE_CONTROL_IsCoop(haloENGINE_CONTROL *self);

/* dsTSTRING<char> operations (ws-engine boundary; free-function form of the C++ thiscall/operators) */
extern void      dsTSTRING_UnsafeInitEmpty(dsTSTRING_flat *out);                          /* = shared empty, +1 ref */
extern void      dsTSTRING_ReleaseBuffer(dsTSTRING_flat *s, dsTSTRING_BUF_HEADER<char> *buf);
extern void      dsTSTRING_Insert(dsTSTRING_flat *s, int at, const char *src, int len);   /* Insert(int, char const *, int) */
extern void      dsTSTRING_InsertString(dsTSTRING_flat *s, int at, const dsTSTRING_flat *src);  /* Insert(int, dsTSTRING_flat const &) */
extern void      dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);              /* operator= */
extern void      dsTSTRING_dtor(dsTSTRING_flat *s);                                        /* ~dsTSTRING_flat */
extern dsTSTRING_flat *fnmGetNameStr(dsTSTRING_flat *out, const dsTSTRING_flat *path);               /* strip to base name */

extern "C" int  verify_dir(const dsTSTRING_flat *dir);   /* HCEX sibling */
extern void _apLog(const char *format, ...);

/* function-local-static lazy-init machinery (ws-engine / CRT boundary) */
extern unsigned int hcex_get_autosave_name_guard;      /* _S2 */
extern void hcex_get_autosave_name_path_dtor(void);
extern "C" int  atexit(void (*func)(void));

extern "C" const char *hcex_chpt_ext;   /* checkpoint file extension ("sav") */

extern "C" char *hcex_get_autosave_name(const char *levelPath)
{
    static dsTSTRING_flat autosave_path;   /* returned buffer lives here across calls */

    if ( (hcex_get_autosave_name_guard & 1) == 0 )
    {
        hcex_get_autosave_name_guard |= 1u;
        autosave_path.pBuffer = 0;
        apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 98);
        dsTSTRING_UnsafeInitEmpty(&autosave_path);
        atexit(&hcex_get_autosave_name_path_dtor);
    }

    if ( !levelPath )
    {
        _apLog("~HCEX,Error~hcex_get_autosave_name(NULL)");
        return autosave_path.pBuffer->str;
    }

    /* start from the checkpoints root directory (ref-counted copy) */
    const dsTSTRING_flat *path_to_checkpoints = haloENGINE_CONTROL_GetPathToCheckpoints(haloEngineCtrl);
    dsTSTRING_flat dir;
    dir.pBuffer = 0;
    dsTSTRING_BUF_HEADER<char> *root_buffer = path_to_checkpoints->pBuffer;
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 883);
    dir.pBuffer = root_buffer;
    ++root_buffer->refCount;
    if ( !verify_dir(&dir) )
    {
        dsTSTRING_ReleaseBuffer(&dir, dir.pBuffer);
        return 0;
    }

    /* append the level's base name */
    dsTSTRING_flat level_path_str;
    level_path_str.pBuffer = 0;
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 149);
    dsTSTRING_UnsafeInit(&level_path_str, levelPath, -1, 0);
    dsTSTRING_flat level_name_str;
    const dsTSTRING_flat *level_name = fnmGetNameStr(&level_name_str, &level_path_str);
    dsTSTRING_InsertString(&dir, dir.pBuffer->strLen, level_name);
    dsTSTRING_ReleaseBuffer(&level_name_str, level_name_str.pBuffer);
    dsTSTRING_ReleaseBuffer(&level_path_str, level_path_str.pBuffer);
    if ( !verify_dir(&dir) )
    {
        dsTSTRING_ReleaseBuffer(&dir, dir.pBuffer);
        return 0;
    }

    /* in co-op, descend into the "\coop" subdirectory */
    if ( haloENGINE_CONTROL_IsCoop(haloEngineCtrl) )
    {
        dsTSTRING_Insert(&dir, dir.pBuffer->strLen, "\\coop", -1);
        if ( !verify_dir(&dir) )
        {
            dsTSTRING_ReleaseBuffer(&dir, dir.pBuffer);
            return 0;
        }
    }

    /* compose "<dir>\<chptName>.<chptExt>" into the persistent result */
    char *dir_str = dir.pBuffer->str;
    const char *chpt_ext = hcex_chpt_ext;
    const char *chpt_name = dbgVAR_STRING_CStr(&dbg_hcex_ChptName);
    dsTSTRING_flat composed;
    dsTSTRING_flat *formatted = dsSPrintf(&composed, "%s\\%s.%s", dir_str, chpt_name, chpt_ext);
    dsTSTRING_assign(&autosave_path, formatted);
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 1014);
    if ( !--composed.pBuffer->refCount )
        dlFree(composed.pBuffer);

    /* record "_autosave" into the "0HCEX/checkpoints/name" debug string variable */
    dsTSTRING_flat autosave_value;
    autosave_value.pBuffer = 0;
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 149);
    dsTSTRING_UnsafeInit(&autosave_value, "_autosave", -1, 0);
    dsTSTRING_flat chpt_var_name;
    chpt_var_name.pBuffer = 0;
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 149);
    dsTSTRING_UnsafeInit(&chpt_var_name, "0HCEX/checkpoints/name", -1, 0);
    dbgVAR_MANAGER *manager = dbgVAR_MANAGER_GetManager();
    dbgVAR_MANAGER_SetDbgVar_tstring(manager, &chpt_var_name, &autosave_value);
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 1014);
    if ( !--chpt_var_name.pBuffer->refCount )
        dlFree(chpt_var_name.pBuffer);
    apPROCESSOR_LOCK_CheckCurrentProcessor(&dsTSTRING_processorLock, DS_STRING_HPP, 1014);
    if ( !--autosave_value.pBuffer->refCount )
        dlFree(autosave_value.pBuffer);

    char *result = autosave_path.pBuffer->str;
    dsTSTRING_dtor(&dir);
    return result;
}
