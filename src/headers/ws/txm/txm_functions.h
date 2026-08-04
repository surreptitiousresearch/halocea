#pragma once
#include "ENGINE_TEX_DESC.h"
#include "../ps/psSECTION_INTERFACE.h"
#include "../../d3d_boundary.h"
// ws-engine txm: free functions bridging the engine texture-usage/format model to the D3D SDK
// and to the ".td" script format.

// 0x827BA828 -- reversed in txmPctFmtToD3dFmt.c. Maps a pct (picture library) pixel format enum
// to the closest D3DFORMAT, using the tiled/"LIN_" variant unless `isSwizzledFormatReq` is set
// (in which case the swizzled/untiled sibling -- offset by 0x186 from the LIN_ base -- is
// returned for the formats that support both layouts).
extern _D3DFORMAT txmPctFmtToD3dFmt(int pctFmt, int isSwizzledFormatReq);

// 0x827BBC48 -- reversed in txmCalcTexureSizeHW.c. Computes the packed/tiled size in bytes of a
// hardware texture via the XDK's XGSetTextureHeader (querying only pBaseSize, out-params for the
// texture object/pitch/mip-size are unused/null).
extern unsigned int txmCalcTexureSizeHW(unsigned int sizeX, unsigned int sizeY,
                                         unsigned int mipCount, int pctFormat);

// 0x82C65C80 -- reversed in txmTexDescGetSRGB.c. True when `desc` selects an sRGB-encoded usage
// (diffuse/lightmap/albedo-family bit 0x1 or 0x10000 set) and rendering.linearRGB is not forced.
extern bool txmTexDescGetSRGB(const ENGINE_TEX_DESC &desc);

// 0x82C66558-adjacent (?) -- 0x82C6655C -- reversed in txmParseTexUsage.c. Parses a "+"-joined
// usage string (e.g. "diffuse+normal") into *usage via ds::Tokenize<TEX_USAGE_PROCESSOR>. Returns
// false on a tokenize error (leaving *usage untouched).
extern bool txmParseTexUsage(ENGINE_TEX_DESC::USAGE &usage, const char *usage_str);

// 0x82C66648 -- reversed in txmTexDescLoad.c. Parses a ".td" script section (`src`) into *result.
// Returns false if `src` is a null section handle or its "version" key isn't 1.0. On success,
// fills usage/mapping/rendering/isUltraHiRes/strmNoLowRes/strmPriority from the section's keys
// (each individually optional; missing keys keep the field's pre-set default).
extern bool txmTexDescLoad(ENGINE_TEX_DESC *result, const psSECTION_INTERFACE &src);
