#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::GetNavSys (non-const) @ 0x82D319D0
// ?GetNavSys@aiEXIFACE@@QAAPAVnavSYS@@F@Z
navSYS *aiEXIFACE::GetNavSys(short nsID)
{
    navMANAGER *mgr = aiPlanner->GetNavManager();
    if (mgr)
        return mgr->GetNavSys(nsID);
    return nullptr;
}

// aiEXIFACE::GetNavSys (const) @ 0x82D31A38
// ?GetNavSys@aiEXIFACE@@QBAPBVnavSYS@@F@Z
// Identical body to the non-const overload (the const-ness only affects the returned pointer).
const navSYS *aiEXIFACE::GetNavSys(short nsID) const
{
    navMANAGER *mgr = aiPlanner->GetNavManager();
    if (mgr)
        return mgr->GetNavSys(nsID);
    return nullptr;
}
