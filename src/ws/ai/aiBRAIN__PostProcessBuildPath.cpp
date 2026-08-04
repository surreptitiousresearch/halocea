#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBEST_BASE.h" // mind.best.pointee->PostProcessBuildPath()

// aiBRAIN::PostProcessBuildPath(navSYS*) @ 0x832ADDB0
// ?PostProcessBuildPath@aiBRAIN@@UAAXPAVnavSYS@@@Z
//
// Virtual nav hook: when `ns` is this brain's own nav system, forward the post-build-path notice to
// the best-behaviour module.
void aiBRAIN::PostProcessBuildPath(navSYS *ns)
{
    if (ns && ns == GetNavSystem())
        mind.best.pointee->PostProcessBuildPath();
}
