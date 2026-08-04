#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"

// propHALO_OBJ_DESC::~propHALO_OBJ_DESC() @ 0x823EB3C8
// Reset the vtable back to propBASE_DESC's own (standard destructor vptr unwind), tear down
// sslScripts and sslClass, then destroy the dscDESC base.
propHALO_OBJ_DESC::~propHALO_OBJ_DESC()
{
    this->__vftable = (dscDESC_vtbl *)&propBASE_DESC::vftable;
    this->sslScripts.~dsVECTOR<dsTSTRING<char>, 8>();
    this->sslClass.~sslCLASS_REF();
    // Base dscDESC subobject destruction happens implicitly at end of scope (propBASE_DESC's own
    // dtor, then dscDESC's, per the compiler-generated destructor chain — matches the decompile's
    // explicit `dscDESC::~dscDESC(this)` tail call).
}
