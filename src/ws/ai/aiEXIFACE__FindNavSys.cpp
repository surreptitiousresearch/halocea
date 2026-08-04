#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::FindNavSys @ 0x82D31968
// ?FindNavSys@aiEXIFACE@@QAAPAVnavSYS@@PBD@Z
navSYS *aiEXIFACE::FindNavSys(const char *nameSys)
{
    navMANAGER *mgr = aiPlanner->GetNavManager();
    if (mgr)
        return mgr->FindNavSys(nameSys);
    return nullptr;
}
