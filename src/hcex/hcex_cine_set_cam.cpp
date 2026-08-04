/* hcex_cine_set_cam @0x823BA1D0 — HCEX debug cinematic-camera dump hook: while a debug cine capture is
 * active and dbg_hcex_DumpCine is on, record the camera's world matrix into the per-actor debug map
 * (keyed by the sentinel name "$camera"), lazily creating its entry (and output file
 * <_dbgCineInfo.path>\cine_log_camera.txt) on first use. When dbg_hcex_DumpCineExtras is also on,
 * additionally append a running frame/time/delta-time line to cine_log_camera_time.log.
 *
 * DEVIATION: the decompiler renders the `totaltime`/`gsElapsedTime` varargs as 64-bit
 * HIDWORD/COERCE_UNSIGNED_INT64 reinterpretations of a `frame` int — a known PPC-decompiler artifact of
 * spilling FPR varargs (both are loaded with `lfs`, a single-precision load, confirmed via disasm at
 * 0x823BA4D8x0x823BA524h). The real call, matching the parallel pattern in hcex_cine_on_play_sound, is
 * `_apForceLog(path, "frame = %i, t = %4.4f , dt = %4.4f\n", _dbgCineInfo.frame, totaltime, gsElapsedTime)`. */

#include "../headers/hcex/hcex_cine_set_cam_debug_boundary.h"

void hcex_cine_set_cam(camCAMERA *pCamera)
{
    if ( !(dbg_hcex_DumpCine.value && hcex_is_cine_mode() && _dbgCineInfo.isInited) )
        return;

    dsSTRID camKey;
    dsSTRID_ctor(&camKey, "$camera", 0);
    DBG_CINE_ACTOR_MAP_ITER it = DBG_CINE_INFO_mapActors_Find(&_dbgCineInfo.mapActors, &camKey);

    if ( !it.item )
    {
        dsTSTRING_flat camLogPath;
        camLogPath.pBuffer = 0;
        dsTSTRING_concat_cstr(&camLogPath, &_dbgCineInfo.path, "\\cine_log_camera.txt");

        DBG_CINE_ACTOR_INFO newEntry;
        DBG_CINE_ACTOR_INFO_ctor(&newEntry, &camLogPath, _dbgCineInfo.timeFromStart);

        if ( --camLogPath.pBuffer->refCount == 0 )
            dlFree(camLogPath.pBuffer);

        dsSTRID_ctor(&camKey, "$camera", 0);
        it = DBG_CINE_INFO_mapActors_FindInsert(&_dbgCineInfo.mapActors, &camKey, &newEntry);

        if ( --newEntry.filePath.pBuffer->refCount == 0 )
            dlFree(newEntry.filePath.pBuffer);
    }

    DBG_CINE_ACTOR_INFO_DumpMatrix(DBG_CINE_INFO_mapActors_Deref(it), &pCamera->matrC2W);

    if ( dbg_hcex_DumpCineExtras.value )
    {
        dsTSTRING_flat timeLogPath;
        timeLogPath.pBuffer = 0;
        dsTSTRING_concat_cstr(&timeLogPath, &_dbgCineInfo.path, "\\cine_log_camera_time.log");

        _apForceLog(timeLogPath.pBuffer->str, "frame = %i, t = %4.4f , dt = %4.4f\n",
                    _dbgCineInfo.frame, (double)totaltime, (double)gsElapsedTime);

        if ( --timeLogPath.pBuffer->refCount == 0 )
            dlFree(timeLogPath.pBuffer);

        totaltime = totaltime + gsElapsedTime;
    }
}
