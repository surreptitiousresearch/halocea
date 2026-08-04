#include "wb_boundary.h"

namespace wb {

// wb::IsActive(int) @ 0x8269E1F0 -- attributes: thunk; a bare tail-call to the bounds-checked
// GetVarValue helper (not one of this batch's targets -- see wb_boundary.h).
bool IsActive(int page)
{
    return wb::GetVarValue(page);
}

} // namespace wb
