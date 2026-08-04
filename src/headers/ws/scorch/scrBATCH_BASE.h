#pragma once
#include "../ds/dsVECTOR.h"
#include "../vid/PASS_DESC_DECAL.h"
#include "../vid/vidPASS_CONFIG.h"
// ws-engine scorch: the geometry+render state shared by static and dynamic scorch batches.
// DB-verified layout (types_members scrBATCH_BASE) -- size 356.

struct scrV3D;    // scorch vertex (position/uv/color) -- boundary (dsVECTOR element, pointer only)
struct vidSHADER; // ../vid shader object              -- boundary (pointer only)

typedef struct scrBATCH_BASE {
    dsVECTOR<scrV3D, 16>          vtxList;              // 0x00 (20)
    dsVECTOR<unsigned short, 16>  indList;              // 0x14 (20)
    PASS_DESC_DECAL               vidDescPass;          // 0x28 (40)
    int                           vidFlags;             // 0x50
    vidPASS_CONFIG                vidCfgColor;          // 0x54 (264)
    vidSHADER                    *vidColorVertexShader; // 0x15C
    vidSHADER                    *vidColorPixelShader;  // 0x160
} scrBATCH_BASE;                                        // 356 bytes
