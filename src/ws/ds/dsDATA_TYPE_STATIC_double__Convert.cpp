#include "ds_boundary.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"

// ds_data::dsDATA_TYPE_STATIC<double>::Convert @ 0x8256E8C0
// Convert a value of runtime type `type` into a double, storing into `dest`. Looks up the
// (double, srcType) cell of the global conversion matrix; if the requested direction is enabled
// and a converter exists, run it — otherwise report failure (0).
// Row index 10 is double's own destination type-id: disasm computes cell base 0x8C00
// (= 10 rows * 32 cells * 0x70-byte stride) at 0x8256E904.
template<>
int ds_data::dsDATA_TYPE_STATIC<double>::Convert(const unsigned int *src, unsigned int *dest,
                                                 const dsDATA_TYPE *type, dsCONVERTION_TYPE convType)
{
    int srcTypeId = type->GetTypeId();
    if (srcTypeId) {
        const dsT2T_ENTRY &cell = t2tTable[10][srcTypeId];
        if ((cell.convertFlags & (1u << convType)) != 0 && cell.converter != nullptr)
            return cell.converter(src, dest);
    }
    return 0;
}
