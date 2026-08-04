#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBEST_BASE.h" // mind.best.pointee->PreProcessBuildPath()

// aiBRAIN::PreProcessBuildPath(navSYS*) @ 0x832ADD48
// ?PreProcessBuildPath@aiBRAIN@@UAAXPAVnavSYS@@@Z
//
// Virtual nav hook: when `ns` is this brain's own nav system, forward the pre-build-path notice to
// the best-behaviour module.
void aiBRAIN::PreProcessBuildPath(navSYS *ns)
{
    if (ns && ns == GetNavSystem())
        mind.best.pointee->PreProcessBuildPath();
}
