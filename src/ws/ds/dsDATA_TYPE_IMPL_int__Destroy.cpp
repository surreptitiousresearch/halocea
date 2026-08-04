#include "dsDATA_TYPE_IMPL.h"

// ds_data::dsDATA_TYPE_IMPL<int>::Destroy @ 0x8257F1C0
// Clear the inline int slot.
template<>
void ds_data::dsDATA_TYPE_IMPL<int>::Destroy(unsigned int *storage) const
{
    *storage = 0;
}
