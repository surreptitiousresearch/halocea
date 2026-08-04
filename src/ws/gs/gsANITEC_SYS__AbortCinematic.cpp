#include "../../headers/ws/gs/gsANITEC_SYS.h"

// 0x825B5B20 — gsANITEC_SYS::AbortCinematic
// If a cinematic actor is currently bound, tear down the active cinematic via the
// virtual OnCineEnd(true) hook (the "aborted" end path, as opposed to a natural finish).
void gsANITEC_SYS::AbortCinematic()
{
    if ( pCurActor )
        OnCineEnd(true);
}
