#include "../../headers/ws/cam/camFRUSTUM.h"

// 0x82C45AAC  ?CullAABB@camFRUSTUM@@QBA_NPBM@Z
// Test an axis-aligned box (6 packed min/max floats) against the frustum. Uses the precise
// ("fine") test when the cached AABB is valid and the debug toggle is enabled, otherwise the
// cheaper draft test. Returns true when the box is entirely outside the frustum (culled).
bool camFRUSTUM::CullAABB(const float *boxMinMax) const
{
    if ( this->isAABBValid && dbd_useFineAABBCulling.value )
        return this->CullAABBFine(boxMinMax);
    return this->CullAABBDraft(boxMinMax);
}
