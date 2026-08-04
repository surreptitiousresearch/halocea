#include "dsDATA_TYPE_IMPL.h"
#include "ds_boundary.h"

// ds_data::dsDATA_TYPE_IMPL<double>::Convert @ 0x8257FB58
// Forward to the static conversion policy for double.
template<>
int ds_data::dsDATA_TYPE_IMPL<double>::Convert(const unsigned int *src, unsigned int *dest,
                                               const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const
{
    return ds_data::dsDATA_TYPE_STATIC<double>::Convert(src, dest, type, convType);
}
