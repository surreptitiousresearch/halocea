#pragma once

// Xenos (Xbox 360 GPU) texture-format helpers — external D3D/XDK boundary. Not reversed here;
// only the prototypes consumed by txmD3D_TEX::EnableGamma are declared.

// GPUTEXTUREFORMAT is the Xenos hardware texture-format enum (6-bit DataFormat field of a
// GPUTEXTURE_FETCH_CONSTANT). DB-verified enumerator set in GPUTEXTUREFORMAT.h.
#include "GPUTEXTUREFORMAT.h"

// ?_d3dXenonMake16bppFilteredFormat@@YA?AW4GPUTEXTUREFORMAT@@W41@@Z /
// ?_d3dXenonMake8bppFilteredFormat@@... -- map a base texture format to its hardware-filtered
// 16bpp / 8bpp sibling (used to toggle gamma-correct filtering on the GPU fetch constant).
// boundary (external to this batch).
extern GPUTEXTUREFORMAT _d3dXenonMake16bppFilteredFormat(GPUTEXTUREFORMAT fmt);
extern GPUTEXTUREFORMAT _d3dXenonMake8bppFilteredFormat(GPUTEXTUREFORMAT fmt);
