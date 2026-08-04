#include "ds_boundary.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"

// ds_data::dsDATA_TYPE_STATIC<int>::Convert @ 0x8256DC30
// Convert a value of runtime type `type` into an int, storing into `dest`. Looks up the
// (int, srcType) cell of the global conversion matrix; if the requested direction is enabled
// and a converter exists, run it — otherwise report failure (0).
// Row index 6 is int's own destination type-id (constant in the disasm at 0x8256DC68).
template<>
int ds_data::dsDATA_TYPE_STATIC<int>::Convert(const unsigned int *src, unsigned int *dest,
                                              const dsDATA_TYPE *type, dsCONVERTION_TYPE convType)
{
    int srcTypeId = type->GetTypeId();
    if (srcTypeId) {
        const dsT2T_ENTRY &cell = t2tTable[6][srcTypeId];
        if ((cell.convertFlags & (1u << convType)) != 0 && cell.converter != nullptr)
            return cell.converter(src, dest);
    }
    return 0;
}
