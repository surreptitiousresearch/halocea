#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::CallNew<m3dMATR> @ 0x828600E0
// Placement-"construct" helper for a POD element: just returns the raw slot pointer (no ctor to run).
template<>
template<>
m3dMATR *dsVECTOR<m3dMATR, 8>::CallNew<m3dMATR>(m3dMATR *t)
{
    return t;
}
