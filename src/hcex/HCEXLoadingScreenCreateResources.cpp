/* HCEXLoadingScreenCreateResources @0x823F3890 — idempotent lazy-create of the loading screen's
 * D3D resources: vertex/pixel shader, background/mask textures (clamp-addressed), and a 2-element
 * vertex declaration (position + one more attribute, D3DDECL_END-terminated). Returns false (and
 * tears everything back down via HCEXLoadingScreenDestroyResources) if any creation step failed.
 *
 * CAVEAT — `vidDriver->drvInterface->pDevice` (offset 0x128 of vidDRIVER_INTERFACE, right after
 * `dynGeomBuffer`@0x124): the DB has no name for this field; disasm confirms the offset (`lwz r29,
 * 0x128(r8)`) and its use as the `D3DDevice*` passed to D3DXCreateTextureFromFileA, so the name is
 * inferred from usage, not DB-verified. */

#include "../headers/d3d_render_boundary.h"
#include "../headers/d3d_vertex_elements.h"

/* Local minimal vidDRIVER/vidDRIVER_INTERFACE view (matches the per-file boundary-struct
 * convention used elsewhere, e.g. vidDRIVER_DYNGEOM_BUFFER.h) -- only wide enough to reach the
 * D3DDevice pointer this function reads. */
typedef struct vidDRIVER_INTERFACE
{
    unsigned char _opaque0[0x128];
    D3DDevice    *pDevice; /* 0x128 -- name inferred from usage, see file CAVEAT */
} vidDRIVER_INTERFACE;

typedef struct vidDRIVER
{
    unsigned char          _opaque0[0x164];
    vidDRIVER_INTERFACE  *drvInterface; /* 0x164 */
} vidDRIVER;

extern vidDRIVER *vidDriver;

extern int                   sHCEXLoadingScreenResourcesCreated;
extern D3DVertexShader      *spHCEXLoadingScreenVS;
extern D3DPixelShader       *spHCEXLoadingScreenPS;
extern D3DVertexDeclaration *spHCEXLoadingScreenVertexDecl;
extern D3DTexture           *spHCEXLoadingScreenBackgroundTex;
extern D3DTexture           *spHCEXLoadingScreenMaskTex;
extern const unsigned int    sHcexLoadingScreenCodeVS[]; /* vertex shader bytecode */
extern const unsigned int    sHcexLoadingScreenCodePS[]; /* pixel shader bytecode */
extern const char            HCEX_LOADING_SCREEN_BACKGROUND_TEX[];
extern const char            HCEX_LOADING_SCREEN_MASK_TEX[];

extern D3DVertexShader      *D3DDevice_CreateVertexShader(const unsigned int *function);
extern D3DPixelShader       *D3DDevice_CreatePixelShader(const unsigned int *function);
extern D3DVertexDeclaration *D3DDevice_CreateVertexDeclaration(const D3DVERTEXELEMENT9 *elements);
extern int                   D3DXCreateTextureFromFileA(D3DDevice *pDevice, const char *pSrcFile, D3DTexture **ppTexture);

extern void HCEXLoadingScreenDestroyResources(void);

/* Xbox 360 GPU texture-fetch address-mode bits: set U/V/W addressing to clamp (bits 2-3, 4-5,
 * 6-7 of fetch-constant dword0) and the aniso/mip filter bits of dword1 (rol(25,1)&0x3F == 0x32,
 * preserving the low 6 bits and leaving the rest of dword1 untouched). */
static void hcexSetClampAddressing(D3DTexture *tex)
{
    tex->base.Format.dword[0] |= 0xCu;
    tex->base.Format.dword[0] |= 0x30u;
    tex->base.Format.dword[0] |= 0xC0u;
    tex->base.Format.dword[1] = (0x32u & 0x3Fu) | (tex->base.Format.dword[1] & 0xFFFFFFC0u);
}

int HCEXLoadingScreenCreateResources(void)
{
    D3DDevice *device;
    D3DVertexDeclaration *decl;
    D3DVERTEXELEMENT9 elements[3];
    int ok;

    if (sHCEXLoadingScreenResourcesCreated)
        return 1;
    sHCEXLoadingScreenResourcesCreated = 1;

    ok = 1;
    device = vidDriver->drvInterface->pDevice;

    spHCEXLoadingScreenVS = D3DDevice_CreateVertexShader(sHcexLoadingScreenCodeVS);
    spHCEXLoadingScreenPS = D3DDevice_CreatePixelShader(sHcexLoadingScreenCodePS);

    if (D3DXCreateTextureFromFileA(device, HCEX_LOADING_SCREEN_BACKGROUND_TEX, &spHCEXLoadingScreenBackgroundTex) < 0)
        ok = 0;
    else
        hcexSetClampAddressing(spHCEXLoadingScreenBackgroundTex);

    if (D3DXCreateTextureFromFileA(device, HCEX_LOADING_SCREEN_MASK_TEX, &spHCEXLoadingScreenMaskTex) < 0)
        ok = 0;
    else
        hcexSetClampAddressing(spHCEXLoadingScreenMaskTex);

    /* element[0]: Stream=0, Offset=0, Type=2761657 (Xbox 360 packed float2 position token), Method=0, Usage=0, UsageIndex=0 */
    elements[0].Stream = 0;
    elements[0].Offset = 0;
    elements[0].Method = 0;
    elements[0].Usage = 0;
    elements[0].Type = 2761657;
    elements[0].UsageIndex = 0;
    /* element[1]: Stream=0, Offset=12, Type=2892709 (packed float2 texcoord token), Method=0, Usage=5 (TEXCOORD), UsageIndex=0 */
    elements[1].Stream = 0;
    elements[1].Offset = 12;
    elements[1].Type = 2892709;
    elements[1].Method = 0;
    elements[1].Usage = 5;
    elements[1].UsageIndex = 0;
    /* element[2]: D3DDECL_END() sentinel {0xFF, 0, D3DDECLTYPE_UNUSED(-1), 0, 0, 0} */
    elements[2].Stream = 255;
    elements[2].Offset = 0;
    elements[2].Type = (unsigned int)-1;
    elements[2].Method = 0;
    elements[2].Usage = 0;
    elements[2].UsageIndex = 0;

    decl = D3DDevice_CreateVertexDeclaration(elements);
    spHCEXLoadingScreenVertexDecl = decl;

    if (!decl || !ok)
    {
        HCEXLoadingScreenDestroyResources();
        return 0;
    }
    return 1;
}
