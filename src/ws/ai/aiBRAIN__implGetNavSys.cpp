#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// aiBRAIN::implGetNavSys() @ 0x83247F68
// ?implGetNavSys@aiBRAIN@@UAA?AV?$dsTSTRING@D@@XZ  (virtual, returns dsTSTRING<char> by value)
// SSL op: the name of the nav-system this brain's goal module is currently localized to. The goal
// module lives at brain+0x1F0 (mind.goal.pointee); its vtbl slot 4 returns the nav system, whose
// nameSys buffer supplies the characters. An empty string is returned when there is no nav system.
dsTSTRING<char> aiBRAIN::implGetNavSys()
{
    navSYS *ns = mind.goal.pointee->GetNavSystem();
    const char *name = ns ? ns->nameSys.pBuffer->str : "";

    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInit(name, -1, 0);
    return result;
}
