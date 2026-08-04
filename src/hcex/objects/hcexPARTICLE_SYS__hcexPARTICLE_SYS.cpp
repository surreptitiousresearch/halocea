/* hcexPARTICLE_SYS::hcexPARTICLE_SYS @0x823CF098 — install this class's vftable (no other state
 * to init). */

#include "../../headers/hcex/hcexPARTICLE_SYS.h"

hcexPARTICLE_SYS_vtbl hcexPARTICLE_SYS::vftable; // boundary storage — real contents not modeled

hcexPARTICLE_SYS::hcexPARTICLE_SYS()
{
    __vftable = reinterpret_cast<gsPARTICLE_SYS_vtbl *>(&hcexPARTICLE_SYS::vftable);
}
