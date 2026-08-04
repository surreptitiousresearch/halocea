/* hcex_get_global_d3d_device @ 0x823EFB18
   Accessor: returns the active D3D device from the global d3dDRIVER. */

#include "../headers/d3d_driver.h"

extern "C" D3DDevice *hcex_get_global_d3d_device(void)
{
    return d3dDriver->pDevice;
}
