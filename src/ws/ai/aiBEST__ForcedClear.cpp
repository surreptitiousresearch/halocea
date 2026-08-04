#include "../../headers/ws/ai/aiBEST.h"

// aiBEST::ForcedClear @ 0x832984B0
// ?ForcedClear@aiBEST@@UAAXXZ  (virtual override)
//
// Force-clear the whole best-behaviour module: note the reason in the debug line, erase the entire
// changeable stack (reason BHV_ST_MANUALLY_CANCELLED), then commit the pending changes.
void aiBEST::ForcedClear()
{
    dbgLine = dsTSTRING<char>("ForcedClear() is in process.");
    EraseAll(BHV_ST_MANUALLY_CANCELLED);
    CommitChanges();
}
