#pragma once
// ws-engine vid: user-facing video settings. Full DB-verified layout (types_members
// vidCONFIG, size 196) with every nested quality enum's DB-verified enumerator set
// (types_enum_values vidCONFIG::*).

typedef struct vidCONFIG
{
    enum MATERIAL_QUALITY     { MTL_LOW = 0, MTL_HI = 1 };
    enum PARALLAX_QUAL        { PRLX_OFF = 0, PRLX_DEFAULT = 1 };
    enum DECALS_QUAL          { DECALS_OFF = 0, DECALS_SIMPLE = 1, DECALS_PARALLAXED = 2 };
    enum HDR_QUALITY          { HDR_OFF = 0, HDR_LOW = 1, HDR_HI = 2 };
    enum ANISOTROPY_QUAL      { ANISO_OFF = 0, ANISO_2X = 1, ANISO_4X = 2, ANISO_8X = 3, ANISO_16X = 4 };
    enum SHADOW_MAP_QUALITY   { SHDMAP_OFF = 0, SHDMAP_LOW = 1, SHDMAP_MID = 2, SHDMAP_HIGH = 3 };
    enum SHADOW_MAP_RESOLUTION{ SHDMAPRES_HIGH = 0, SHDMAPRES_MID = 1, SHDMAPRES_LOW = 2 };
    enum SHADOW_QUALITY       { SHD_OFF = 0, SHD_NORMAL = 1, SHD_GOOD = 2 };
    enum VSYNC_FORCE          { VSYNC_FORCE_ANY = 0, VSYNC_FORCE_OFF = 1, VSYNC_FORCE_ON = 2 };
    enum SHATTER_QUALITY      { STR_LOW = 0, STR_HIGH = 1 };
    enum AA_QUALITY           { AA_OFF = 0, AA_MSAA2X = 1 };
    enum TEXTURE_RES          { TEXRES_LOW = 0, TEXRES_GOOD = 1, TEXRES_BEST = 2 };
    enum NM_COMPRESSION       { NM_COMPRESS_NORMAL = 0, NM_COMPRESS_BEST = 1 };
    enum TREE_QUALITY         { TREE_LOW = 0, TREE_NORMAL = 1, TREE_GOOD = 2 };
    enum VIDSETTINGS          { SETTINGS_DEFAULT = 0, SETTINGS_MULTIPLAYER = 1, SETTINGS_FMV = 2 };

    MATERIAL_QUALITY      materialQuality;       /* 0x00 */
    PARALLAX_QUAL         parallaxQuality;       /* 0x04 */
    int                   useParallaxShadows;    /* 0x08 */
    DECALS_QUAL           decalsQuality;         /* 0x0C */
    HDR_QUALITY           hdrQuality;            /* 0x10 */
    int                   useDOF;                /* 0x14 */
    int                   useSSAO;               /* 0x18 */
    int                   useDistortion;         /* 0x1C */
    ANISOTROPY_QUAL       anisotropyQuality;     /* 0x20 */
    SHADOW_MAP_QUALITY    shadowMapQuality;      /* 0x24 */
    SHADOW_MAP_RESOLUTION shadowMapResolution;   /* 0x28 */
    int                   smoothShadowMaps;      /* 0x2C */
    int                   useVSync;              /* 0x30 (48) */
    VSYNC_FORCE           forceVSync;            /* 0x34 (52) */
    float                 gamma;                 /* 0x38 */
    float                 brightness;            /* 0x3C */
    float                 contrast;              /* 0x40 */
    float                 saturation;            /* 0x44 */
    int                   mode;                  /* 0x48 */
    int                   mode_sx;               /* 0x4C */
    int                   mode_sy;               /* 0x50 */
    int                   mode_bpp;              /* 0x54 */
    int                   fullscreen;            /* 0x58 */
    SHATTER_QUALITY       shatterQual;           /* 0x5C */
    AA_QUALITY            AAQual;                /* 0x60 */
    TEXTURE_RES           texRes;                /* 0x64 */
    int                   useTrilinearFiltering; /* 0x68 */
    int                   useDetailTextures;     /* 0x6C */
    int                   useFlares;             /* 0x70 */
    int                   useHDRDebug;           /* 0x74 */
    int                   useSpecular;           /* 0x78 */
    int                   useDiffuse;            /* 0x7C */
    int                   useReflections;        /* 0x80 */
    NM_COMPRESSION        nmCompression;         /* 0x84 */
    int                   useGrass;              /* 0x88 */
    TREE_QUALITY          treeQual;              /* 0x8C */
    int                   ignoreVidMemCheck;     /* 0x90 */
    int                   tripleBuffering;       /* 0x94 */
    int                   useMirrors;            /* 0x98 */
    int                   useDynLights;          /* 0x9C */
    int                   showDbgDiffuseLights;  /* 0xA0 */
    int                   dbgShadowMapType;      /* 0xA4 */
    int                   useSSAONormals;        /* 0xA8 */
    float                 shaderLodDistance;     /* 0xAC */
    float                 SSAOAmount;            /* 0xB0 */
    float                 SSAOAmbient;           /* 0xB4 */
    int                   stereo3D;              /* 0xB8 */
    float                 stereo3DSeparation;    /* 0xBC */
    float                 stereo3DConvergence;   /* 0xC0 */

    // 0x82BE9778 -- reversed in vidCONFIG__GetVSync.c. VSYNC_FORCE_OFF/ON hard-override
    // useVSync; VSYNC_FORCE_ANY defers to it.
    int GetVSync() const;
} vidCONFIG; /* 196 bytes */
