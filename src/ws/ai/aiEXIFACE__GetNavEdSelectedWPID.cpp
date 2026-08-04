#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::GetNavEdSelectedWPID @ 0x82D313F8
// ?GetNavEdSelectedWPID@aiEXIFACE@@QBAHXZ
int aiEXIFACE::GetNavEdSelectedWPID() const
{
    return aiPlanner->GetNavEditor()->GetSelectedWP_ID();
}
