#include "ds_boundary.h" // dsDATA_TYPE_STORAGE<T>::dataType, dsDATA_TYPE

struct sslOBJ_REF; // ssl/sslOBJ_REF.h — script object reference; used only as the storage template arg

// dsDATA_TYPE::GetType<sslOBJ_REF>() @ 0x825233B8
//   lis r11, dataType@ha ; lwz r3, dataType@l(r11) ; blr
// Returns the singleton runtime type descriptor for sslOBJ_REF, i.e.
// dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType. The caller
// (sslSCRIPTED_FUNCTION_BASE::ExecCode) declares this getter as the hand-named
// extern `dsDATA_OBJREF_TYPE`; both name the same one-instruction static load.
const dsDATA_TYPE *dsDATA_OBJREF_TYPE()
{
    return dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType;
}
