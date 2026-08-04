#pragma once

/* Static D3DVERTEXELEMENT9 declaration arrays describing each vertex format.
   These are D3D-boundary descriptor data; treated as opaque arrays.
   DB-verified layout (types_members D3DVERTEXELEMENT9 / _D3DVERTEXELEMENT9): Stream@0 (u16),
   Offset@2 (u16), Type@4 (unsigned int -- note: 4 bytes on this Xbox 360 D3D9, not the PC SDK's
   packed BYTE), Method@8 (u8), Usage@9 (u8), UsageIndex@10 (u8) -- size 12. */
typedef struct D3DVERTEXELEMENT9
{
    unsigned short Stream;     /* 0x0 */
    unsigned short Offset;     /* 0x2 */
    unsigned int   Type;       /* 0x4 */
    unsigned char  Method;     /* 0x8 */
    unsigned char  Usage;      /* 0x9 */
    unsigned char  UsageIndex; /* 0xA */
} D3DVERTEXELEMENT9;

extern D3DVERTEXELEMENT9 d3dve_environment[];
extern D3DVERTEXELEMENT9 d3dve_environment_lightmap[];
extern D3DVERTEXELEMENT9 d3dve_model[];
extern D3DVERTEXELEMENT9 d3dve_unlit[];
extern D3DVERTEXELEMENT9 d3dve_screen[];
extern D3DVERTEXELEMENT9 d3dve_debug[];
extern D3DVERTEXELEMENT9 d3dve_decal[];
extern D3DVERTEXELEMENT9 d3dve_detail_object[];
extern D3DVERTEXELEMENT9 d3dve_unlit_zsprite[];
extern D3DVERTEXELEMENT9 d3dve_widget[];
