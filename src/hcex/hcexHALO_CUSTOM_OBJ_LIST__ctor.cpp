// hcexHALO_CUSTOM_OBJ_LIST::hcexHALO_CUSTOM_OBJ_LIST() @ 0x823D952C
//
// Register this class's vftable, then default-construct cdList as an empty vector adopting the
// "halo_main.cpp":222 allocation call-site cookie.

#include "../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"

hcexHALO_CUSTOM_OBJ_LIST::hcexHALO_CUSTOM_OBJ_LIST()
    : cdList(apCL{"D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 222})
{
    this->__vftable = (instMANAGER_HANDLER_vtbl *)&hcexHALO_CUSTOM_OBJ_LIST::vftable; // base __vftable type
}
