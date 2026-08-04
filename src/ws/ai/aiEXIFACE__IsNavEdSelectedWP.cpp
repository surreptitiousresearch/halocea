#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::IsNavEdSelectedWP @ 0x82D31440
// ?IsNavEdSelectedWP@aiEXIFACE@@QBA_NF@Z
bool aiEXIFACE::IsNavEdSelectedWP(short wpID) const
{
    if (!aiPlanner)
        return false;
    if (!aiPlanner->GetNavEditor())
        return false;
    return aiPlanner->GetNavEditor()->IsSelectedWP_2(wpID);
}
