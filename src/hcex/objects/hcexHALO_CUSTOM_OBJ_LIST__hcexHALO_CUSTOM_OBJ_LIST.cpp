/* hcexHALO_CUSTOM_OBJ_LIST::hcexHALO_CUSTOM_OBJ_LIST @0x823D952C — install this class's vftable
 * and default-construct cdList (adopting the "halo_main.cpp":222 allocation call-site cookie). */

#include "../../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"

hcexHALO_CUSTOM_OBJ_LIST_vtbl hcexHALO_CUSTOM_OBJ_LIST::vftable; // boundary storage — real contents not modeled

hcexHALO_CUSTOM_OBJ_LIST::hcexHALO_CUSTOM_OBJ_LIST()
{
    __vftable = (instMANAGER_HANDLER_vtbl *)&hcexHALO_CUSTOM_OBJ_LIST::vftable; // base __vftable type

    apCL cl;
    cl.file = "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp";
    cl.line = 222;

    cdList.pData = 0;
    cdList.nElem = 0;
    cdList.allocated = 0;
    cdList.__cl = cl;
}
