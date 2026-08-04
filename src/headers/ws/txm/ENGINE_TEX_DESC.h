#pragma once
// ws-engine txm: the engine-facing texture descriptor -- usage bitmask, sampler mapping
// state, and misc rendering hints. Embedded by value in txmTEXTURE::desc and loaded from
// ".td" script files by txmTexDescLoad / txmTEX_DESC_DB.
// DB-verified layout (types_members ENGINE_TEX_DESC): usage@0 (USAGE, 8B), mapping@8
// (MAPPING, 20B), rendering@28 (RENDERING, 20B), strmPriority@48 (ENGINE_TEX_STRM_PRIORITY),
// isUltraHiRes@52 (bool), strmNoLowRes@53 (bool) -- size 56.

// DB-verified (types_enum_values ENGINE_TEX_ADDRESS_MODE): WRAP=0, CLAMP=1, MIRROR=2, LAST=3.
// NOTE: distinct from the placeholder `ETADDR` enum in txm_boundary.h (which mis-numbers
// CLAMP=0); this is the real engine enum used by ENGINE_TEX_DESC::MAPPING.
typedef enum ENGINE_TEX_ADDRESS_MODE
{
    ETADDR_WRAP   = 0,
    ETADDR_CLAMP  = 1,
    ETADDR_MIRROR = 2,
    ETADDR_LAST   = 3,
} ENGINE_TEX_ADDRESS_MODE;

// DB-verified (types_enum_values ENGINE_TEX_STRM_PRIORITY): LOW=0, NORMAL=1, HIGH=2, NOT_STREAM=3.
typedef enum ENGINE_TEX_STRM_PRIORITY
{
    ETSP_LOW        = 0,
    ETSP_NORMAL     = 1,
    ETSP_HIGH       = 2,
    ETSP_NOT_STREAM = 3,
} ENGINE_TEX_STRM_PRIORITY;

typedef struct ENGINE_TEX_DESC
{
    // Bitmask of texture-usage roles (diffuse/normal/specular/... -- see ENGINE_MAP_TYPE_NAME
    // string table) plus a running count of set bits. `maps` bit layout is defined by the
    // ENGINE_MAP_TYPE_NAME token table consumed in TEX_USAGE_PROCESSOR::ProcessToken.
    struct USAGE
    {
        unsigned int maps;    /* 0x00 */
        int          numMaps; /* 0x04 */
    } usage; /* 0x00 */

    struct MAPPING
    {
        float                   lod_bias;             /* 0x00 */
        int                     anisotropy;            /* 0x04 */
        ENGINE_TEX_ADDRESS_MODE addressU;               /* 0x08 */
        ENGINE_TEX_ADDRESS_MODE addressV;               /* 0x0C */
        unsigned short          lod_bias_PS3_cached;    /* 0x10 -- lod_bias*256, clamped to 13 bits */
    } mapping; /* 0x08 */

    struct RENDERING
    {
        int   akillRef;      /* 0x00 alpha-kill reference (0-255) */
        bool  linearRGB;     /* 0x04 */
        bool  smHi;          /* 0x05 */
        float detailDensity; /* 0x08 */
        float detailScale;   /* 0x0C */
        float hdrScale;      /* 0x10 */
    } rendering; /* 0x1C */

    ENGINE_TEX_STRM_PRIORITY strmPriority;  /* 0x30 */
    bool                     isUltraHiRes;  /* 0x34 */
    bool                     strmNoLowRes;  /* 0x35 */

    // 0x82C66AB4 -- reversed in ENGINE_TEX_DESC__Default.c. Function-local-static singleton
    // (usage.maps=0, usage.numMaps=0, strmPriority=ETSP_NORMAL, everything else zeroed by the
    // one-time memset); returns a pointer to it. sret-by-pointer (SAA -- static, returns const&).
    static const ENGINE_TEX_DESC &Default();
} ENGINE_TEX_DESC;
