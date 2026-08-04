// PASS_DESC_FILL::~PASS_DESC_FILL @0x823F0630 — release every texture the gamma/fill pass held.
// Each of the eight txmTEXTURE_PTR members is released (in reverse declaration order) but only while
// the texture manager is still alive (txmManager != null) — this skips releasing during texture-
// manager teardown, matching the sibling scrTEXTURES::~scrTEXTURES pattern. Rendered in the free-
// function form declared by the boundary header. See hcex_fill_backbuffer_boundary.h for the layout.
#include "../headers/hcex/hcex_fill_backbuffer_boundary.h"

void PASS_DESC_FILL_dtor(PASS_DESC_FILL *self)
{
    txmTEXTURE *tex;

    tex = self->texBicubicLookup.ptr; if (tex && txmManager) tex->Release();
    tex = self->texGamma.ptr;         if (tex && txmManager) tex->Release();
    tex = self->texDepth3.ptr;        if (tex && txmManager) tex->Release();
    tex = self->texDepth2.ptr;        if (tex && txmManager) tex->Release();
    tex = self->texDepth1.ptr;        if (tex && txmManager) tex->Release();
    tex = self->texDepth.ptr;         if (tex && txmManager) tex->Release();
    tex = self->texColor1.ptr;        if (tex && txmManager) tex->Release();
    tex = self->texColor.ptr;         if (tex && txmManager) tex->Release();
}
