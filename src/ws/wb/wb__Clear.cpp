#include "wb_boundary.h"

namespace wb {

// wb::Clear(int) @ 0x8269E210 — reset debug page `page`'s accumulated text buffer to empty for the
// next frame. Fetches the bounds-checked page string (wb::GetPage) and clears it in place.
void Clear(int page)
{
    dsTSTRING<char> *pageText = wb::GetPage(page);
    pageText->Clear();
}

} // namespace wb
