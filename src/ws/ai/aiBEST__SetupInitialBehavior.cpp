#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

struct gsDOM_SPAWN; // spawn domain (entENTITY::pDomSpawn pointee)  boundary

// aiBEST::SetupInitialBehavior @ 0x832990B0
// ?SetupInitialBehavior@aiBEST@@UAAXXZ (virtual override)
//
// Reset the stack, then seed the initial behaviour: if this brain was created by a spawn domain and
// that domain supplies a spawn-behaviour name, install it. Fire the mind's SSL OnInit and (timed)
// the brain's OnCreate event, then guarantee the stack is non-empty.
void aiBEST::SetupInitialBehavior()
{
    EraseAll(BHV_ST_MANUALLY_CANCELLED);

    gsDOM_SPAWN *dom = pBrain->pDomSpawn; // entENTITY base member @0x154
    if (dom)
    {
        char nameBuf[64];
        // Domain vtable slot +0x140: write the spawn behaviour name into nameBuf (<=64 bytes);
        // returns nonzero when a name was produced.
        typedef int (*GetSpawnBhvFn)(gsDOM_SPAWN *self, char *buf, int bufSize);
        GetSpawnBhvFn fn = *(GetSpawnBhvFn *)(*(char **)dom + 0x140);
        if (fn(dom, nameBuf, 64))
            SetBehavior(nameBuf);
    }

    pBrain->mind.CallSSL_OnInit();

    {
        static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/init/SSL_OnCreate", nullptr);
        apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);
        pBrain->OnCreate();
    }

    EnsureNonEmpty();
}
