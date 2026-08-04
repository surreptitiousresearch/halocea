#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"

// propHALO_OBJ_DESC::propHALO_OBJ_DESC() @ 0x823EB270
// Base-construct, install this class's vtable, and default the lazily-init'd fields
// (isEntSslClassInited=false, objUse=0, timeTerm=0).
propHALO_OBJ_DESC::propHALO_OBJ_DESC() : propENT_DESC() // immediate base (chains to propBASE_DESC)
{
    this->__vftable = (dscDESC_vtbl *)&propHALO_OBJ_DESC::vftable;
    this->isEntSslClassInited.val = false;
    this->objUse.state = 0;
    this->timeTerm = 0.0f;
}
