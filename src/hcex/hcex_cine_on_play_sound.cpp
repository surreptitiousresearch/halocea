/* hcex_cine_on_play_sound @0x823B5F60 — HCEX cinematic debug hook: when full cine-dump logging is on
 * (dbg_hcex_DumpCine + dbg_hcex_DumpCineExtras) and a debug cinematic capture is active, append the
 * frame number and the sound's full name to <_dbgCineInfo.path>\cine_log_sound.log. Called from
 * hcex_on_play_sound for every sound Blam starts. */

#include "../headers/hcex/hcex_cine_tick_boundary.h"

void hcex_cine_on_play_sound(const char *sound_name_full)
{
    if ( !(dbg_hcex_DumpCine.value && dbg_hcex_DumpCineExtras.value && hcex_is_cine_mode() && _dbgCineInfo.isInited) )
        return;

    dsTSTRING_flat logPath;
    logPath.pBuffer = 0;
    dsTSTRING_concat_cstr(&logPath, &_dbgCineInfo.path, "\\cine_log_sound.log");

    _apForceLog(logPath.pBuffer->str, "frame = %i , sound = %s \n", _dbgCineInfo.frame, sound_name_full);

    if ( --logPath.pBuffer->refCount == 0 )
        dlFree(logPath.pBuffer);
}
