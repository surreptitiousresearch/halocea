/* hcex_set_tex @0x823F0110 — upload a picture into a texture, keeping the running HALO-mode texture
 * memory tally (hcex_halo_tex_size) in sync around the resize, then release the source picture.
 *
 * DEVIATION: the raw decompile rendered the virtual Set call as a zero-argument cast
 * `((void (__fastcall *)(txmTEXTURE *))pTex->Set)(pTex)`, dropping its real arguments. Per disassembly
 * (r4 still holds the caller's `pPict`, r5 is explicitly zeroed with `li r5, 0` right before the call)
 * the true call is `pTex->Set(pTex, pPict, 0)` — vtable slot 19 (offset 0x4C), matching
 * `int (__fastcall*)(txmTEXTURE*, pctPICTURE*, int)`. Also note the `is2d` parameter is dead: r5 is
 * clobbered with a literal 0 immediately after entry, before the incoming value is ever read, so the
 * caller's is2d argument has no effect on this call (reproduced faithfully). */

#include "../headers/hcex/hcex_set_tex_boundary.h"

extern int hcex_halo_tex_size;

void hcex_set_tex(txmTEXTURE *pTex, pctPICTURE *pPict, int is2d)
{
    (void)is2d; /* dead — see deviation note above */

    hcex_halo_tex_size -= pTex->texSize;
    txmTEXTURE__Set(pTex, pPict, 0);
    hcex_halo_tex_size += pTex->texSize;

    pctGetFormat(pPict->hdr.format); /* decl takes format code, not picture */
    pctDestroy(pPict);
}
