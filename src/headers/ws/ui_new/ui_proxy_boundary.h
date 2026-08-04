#pragma once
// Boundary declarations for UI_PROXY__ProcessRender.cpp / UI_PROXY__RenderPush.cpp: the OS/video
// driver free functions and globals they call into but do not themselves reverse (per-consumer
// minimal model, at DB-verified offsets, matching the corpus's established vidDRIVER-boundary
// convention — see e.g. hcex_render_hud_boundary.h).

// canonical DB-verified full layouts (the former per-consumer partial slices were
// consolidated): vidDRIVER_INTERFACE (SetGammaCorrection is virtual dispatch),
// vidMODE (sxRT@0x20 / syRT@0x24), vidDRIVER (pMode@0x10C, drvInterface@0x164).
#include "../../vidDRIVER_INTERFACE.h"
#include "../../vidMODE.h"
#include "../../vidDRIVER.h"

extern vidDRIVER *vidDriver;

extern void osPIXBeginEvent(const char *label);
extern void osPIXEndEvent(void);
