// ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>::~REF_TYPE @ 0x823D7658 -- release this typed handle:
// drop one reference from the backing REF_TYPE_DATA and, at zero refcount, destroy the owned
// dsVECTOR_PARAM_LIST (if the policy is opRefCount) and free the backing cell.
// Unlike REF_TYPE<apCONFIG>::~REF_TYPE, the Release() logic is inlined here rather than delegated.
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsPARAM.h"

template<>
ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>::~REF_TYPE()
{
    ds_data::REF_TYPE_DATA<dsVECTOR_PARAM_LIST> *data = this->m_pData;
    if (--data->m_iRefCount == 0)
    {
        if (data->m_op == ds_data::opRefCount)
        {
            dsVECTOR_PARAM_LIST *owned = data->m_pT;
            if (owned)
            {
                owned->storage.~dsVECTOR<dsPARAM, 8>(); // boundary dtor
                operator delete(owned);
            }
        }
        operator delete(data);
    }
}
