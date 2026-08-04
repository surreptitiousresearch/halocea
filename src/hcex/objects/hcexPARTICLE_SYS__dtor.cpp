/* hcexPARTICLE_SYS::~hcexPARTICLE_SYS @0x823CF0A8 — reset __vftable to the gsPARTICLE_SYS base
 * slot (no owned state to release). */

#include "../../headers/hcex/hcexPARTICLE_SYS.h"

hcexPARTICLE_SYS::~hcexPARTICLE_SYS()
{
    __vftable = &gsPARTICLE_SYS::vftable;
}
