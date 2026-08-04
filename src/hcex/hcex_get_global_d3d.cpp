/* hcex_get_global_d3d @ 0x823EFB28
   Accessor: returns the Direct3D object from the global d3dDRIVER. */

#include "../headers/d3d_driver.h"

extern "C" Direct3D *hcex_get_global_d3d(void)
{
    return d3dDriver->pD3D;
}
