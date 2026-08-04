// dsDATA_NULL_TYPE — out-of-line definition for the inline-only gettype thunk.
// Real DB fn: dsDATA_TYPE::GetType<NULL_TYPE>() @ 0x82516DD8
//   lis  r11, dataType@ha ; lwz r3, dataType@l(r11) ; blr
//   -> return dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType; (static @ 0x8413EB38)
// The template static is the same object used directly in src/ws/ds/dsDATA__GetValue_*.cpp.
// Exposed under the caller's alias signature (extern const dsDATA_TYPE *dsDATA_NULL_TYPE())
// used by src/ws/ssl/sslSCRIPTED_FUNCTION_BASE__ExecCode.cpp to build null dsDATA values.
#include "dsDATA_TYPE.h"
#include "NULL_TYPE.h"
#include "ds_boundary.h"

const dsDATA_TYPE *dsDATA_NULL_TYPE()
{
    return dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType;
}
