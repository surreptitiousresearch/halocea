#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::IsNavEdSelectedEdge @ 0x82D314D8
// ?IsNavEdSelectedEdge@aiEXIFACE@@QBA_NFH@Z
bool aiEXIFACE::IsNavEdSelectedEdge(short nsID, int edgeID) const
{
    if (!aiPlanner)
        return false;
    if (!aiPlanner->GetNavEditor())
        return false;
    return aiPlanner->GetNavEditor()->IsSelectedEdge_2(nsID, edgeID);
}
