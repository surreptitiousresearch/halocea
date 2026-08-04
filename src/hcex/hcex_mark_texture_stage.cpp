/* hcex_mark_texture_stage @0x823F0CD0 — drop a PIX marker naming the texture bound to sampler stage
 * `stage`, of the form "s<stage> <name> <sizeX>x<sizeY>x<sizeZ>". `name` (the trailing const char*
 * parameter) is unused by the disassembly — the label text is read from the texture object itself. */

#include "../headers/hcex/hcex_mark_texture_stage_boundary.h"

void hcex_mark_texture_stage(int stage, txmTEXTURE *tex, const char *name)
{
    (void)name; /* unused — the texture's own name is used instead, see disasm */

    int sizeZ = tex->sizeZ;
    const char *texName = tex->name.pBuffer->str;
    int sizeY = txmTEXTURE__GetSizeY(tex);
    int sizeX = txmTEXTURE__GetSizeX(tex);

    hcex_pix_marker("s%d %s %dx%dx%d", stage, texName, sizeX, sizeY, sizeZ);
}
