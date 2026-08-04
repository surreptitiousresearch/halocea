#pragma once

#include "d3d_boundary.h"

/* Loose rasterizer/engine globals referenced during initialization. */
extern unsigned char use_fullscreen;
extern unsigned char flip_status;
extern unsigned char device_is_lost;
extern unsigned int cf_ForceShader;
extern int rasterizer_initialized;
extern int hcex_screen_width;
extern int hcex_screen_height;

extern _D3DDEVTYPE global_devicetype;
extern D3DDevice *global_d3d_device;
extern Direct3D *global_d3d;
extern _D3DCAPS9 global_d3d_caps;
extern D3DPRESENT_PARAMETERS global_d3d_present_parameters;
