// ds_data::REF_TYPE<apCONFIG>::~REF_TYPE @ 0x823C24C8 — release this typed ref-counted handle by
// dropping its reference to the backing REF_TYPE_DATA (which frees/destroys it at zero refcount).
#include "../../headers/ws/ds/REF_TYPE.h"

struct apCONFIG; // boundary — referenced only through the ref-counted backing cell

template<>
ds_data::REF_TYPE<apCONFIG>::~REF_TYPE()
{
    Release(this->m_pData);
}
