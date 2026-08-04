#pragma once

/* vid* video-driver subsystem boundary for the txm consumers (txmD3D_TEX::GetD3DTex touches
 * vidDriver->pMode->vmState; txmTEXTURE::UpdateTextureSwizzle consults
 * vidDriver->sdrPlatform.dxn). The former per-consumer partial slices of vidMODE /
 * vidPASS_PLATFORM / vidDRIVER were consolidated into the canonical DB-verified headers: */

#include "../../vidMODE.h"
#include "../vid/vidPASS_PLATFORM.h"
#include "../../vidDRIVER.h"

extern vidDRIVER *vidDriver;
