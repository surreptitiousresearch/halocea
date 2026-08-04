#pragma once

/* Xbox 360 D3D resource-type enum (external library boundary type). DB-verified
 * (types_enum_values _D3DRESOURCETYPE). */
typedef enum _D3DRESOURCETYPE
{
    D3DRTYPE_NONE                    = 0,
    D3DRTYPE_VERTEXBUFFER            = 1,
    D3DRTYPE_INDEXBUFFER             = 2,
    D3DRTYPE_TEXTURE                 = 3,
    D3DRTYPE_SURFACE                 = 4,
    D3DRTYPE_VERTEXDECLARATION       = 5,
    D3DRTYPE_VERTEXSHADER            = 6,
    D3DRTYPE_PIXELSHADER             = 7,
    D3DRTYPE_CONSTANTBUFFER          = 8,
    D3DRTYPE_COMMANDBUFFER           = 9,
    D3DRTYPE_ASYNCCOMMANDBUFFERCALL  = 10,
    D3DRTYPE_PERFCOUNTERBATCH        = 11,
    D3DRTYPE_OCCLUSIONQUERYBATCH     = 12,
    D3DRTYPE_VOLUME                  = 16,
    D3DRTYPE_VOLUMETEXTURE           = 17,
    D3DRTYPE_CUBETEXTURE             = 18,
    D3DRTYPE_ARRAYTEXTURE            = 19,
    D3DRTYPE_LINETEXTURE             = 20,
    D3DRTYPE_FORCE_DWORD             = 2147483647,
} _D3DRESOURCETYPE; /* 4 bytes */
