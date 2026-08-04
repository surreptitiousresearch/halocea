// mtlTBL::mtlTBL @0x828835E4 — default-construct: empty mtlList (adopting the material.h:210
// allocation call-site cookie), default texDensity.
#include "../../headers/ws/mtl/mtlTBL.h"

mtlTBL::mtlTBL()
{
    mtlList.pData = nullptr;
    mtlList.nElem = 0;
    mtlList.allocated = 0;
    mtlList.__cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\material.h";
    mtlList.__cl.line = 210;
    // texDensity: default-constructed via its own default ctor (member init list would prefer
    // this; matches the disassembly, which never re-touches texDensity here).
}
