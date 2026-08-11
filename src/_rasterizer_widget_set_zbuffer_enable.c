#include "headers/blam_data_globals.h"
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int zbuffer_enable);

void _rasterizer_widget_set_zbuffer_enable(unsigned char zbuffer_enable)
{
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, zbuffer_enable);
}
