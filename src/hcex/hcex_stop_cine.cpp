/* hcex_stop_cine @0x823BAF08 — abort the currently-playing cinematic and, if a debug cinematic capture was
 * running, tear it down: clear the per-actor capture map, reset the dump-elapsed timer to -1, and reset the
 * global _dbgCineInfo to a fresh default-constructed record.
 *
 * Deviation: the decompiler rendered the reset as `DBG_CINE_INFO::operator=(&_dbgCineInfo, v0)` with an
 * uninitialized v0; the disasm shows a fresh DBG_CINE_INFO is default-constructed on the stack (r3), then
 * assigned into _dbgCineInfo (r3=&_dbgCineInfo, r4=&default), then destroyed — i.e. _dbgCineInfo is reset to
 * default. gsANITEC_SYS / DBG_CINE_INFO are ws-engine boundaries. */

#include "../headers/hcex/DBG_CINE_INFO.h"

struct gsANITEC_SYS;
extern struct gsANITEC_SYS *gsSysAnitec;
extern void gsANITEC_SYS_AbortCinematic(struct gsANITEC_SYS *system);

extern void hcex_unhide_actors(void);
extern float gsElapsedTimeCineDump;
extern DBG_CINE_INFO _dbgCineInfo;

extern "C" void hcex_stop_cine(void)
{
    gsANITEC_SYS_AbortCinematic(gsSysAnitec);
    hcex_unhide_actors();

    if ( _dbgCineInfo.isInited )
    {
        DBG_CINE_INFO default_info;

        DBG_CINE_INFO_mapActors_Clear(&_dbgCineInfo.mapActors);
        gsElapsedTimeCineDump = -1.0f;

        DBG_CINE_INFO_ctor(&default_info);
        DBG_CINE_INFO_assign(&_dbgCineInfo, &default_info);
        DBG_CINE_INFO_dtor(&default_info);
    }
}
