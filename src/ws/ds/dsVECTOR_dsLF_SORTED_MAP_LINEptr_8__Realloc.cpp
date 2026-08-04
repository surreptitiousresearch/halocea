#include "dsVECTOR.h"
#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsSTR_HASH / dsSTR_CMP — string hash/compare policies for the const-char* keyed map.
struct dsSTR_HASH; // boundary
struct dsSTR_CMP;  // boundary

// dsVECTOR<dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE*,8>::Realloc @ 0x8252A5A8
// The hash-table bucket array of the string->strid map. Resize the backing store to hold exactly
// `size` LINE-node pointers. ShrinkResize clamps the live count (the element is a raw pointer,
// trivially destructible), then a single dlRealloc resizes the storage in place. Element size 4 bytes.
template<>
void dsVECTOR<dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::LINE *, 8>::Realloc(int size)
{
    if (size != this->allocated) {
        this->ShrinkResize(size);
        if (!IGNORE_STRONG_ASSERT && size < this->nElem)
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "size >= nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                137,
                "size", size,
                "nElem", this->nElem);
        this->pData = (dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::LINE **)dlRealloc(
            this->pData, 4 * size, this->__cl.file, this->__cl.line);
        this->allocated = size;
    }
}
