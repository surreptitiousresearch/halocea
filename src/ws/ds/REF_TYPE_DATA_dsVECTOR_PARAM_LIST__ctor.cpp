// ds_data::REF_TYPE_DATA<dsVECTOR_PARAM_LIST>::REF_TYPE_DATA @ 0x823D3A68 -- default ctor for the
// ref-counted backing cell of a REF_TYPE<dsVECTOR_PARAM_LIST>: allocate and default-construct a
// fresh dsVECTOR_PARAM_LIST as the owned value, adopt opRefCount ownership, start refcount at 1.
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

template<>
ds_data::REF_TYPE_DATA<dsVECTOR_PARAM_LIST>::REF_TYPE_DATA()
{
    // The disasm inlines dsVECTOR_PARAM_LIST's default ctor: operator new(0x1C) followed by
    // zeroing the 5 leading dwords and stamping the backing dsVECTOR<dsPARAM,8>'s debug
    // (file,line) allocation cookie ("ds_param.h":98). Modeled as a plain new-expression.
    m_pT = new dsVECTOR_PARAM_LIST();
    m_op = opRefCount;
    m_iRefCount = 1;
}
