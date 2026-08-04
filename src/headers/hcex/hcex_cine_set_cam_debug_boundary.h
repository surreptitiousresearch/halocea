#pragma once
/* Boundary declarations for hcex_cine_set_cam.c — the HCEX debug cinematic-camera dump path.
 * camCAMERA/m3dMATR come from the reversed ws-engine headers (only matrC2W is touched here);
 * DBG_CINE_INFO / DBG_CINE_ACTOR_INFO / their mapActors ops come from hcex_cine_tick_boundary.h. */

#include "hcex_cine_tick_boundary.h"
#include "../ws/m3d/m3dMATR.h"
#include "../ws/cam/camCAMERA.h"
