// Flattened C-linkage-style bridge for the hcex boundary headers (hcex_cine_init_boundary.h):
// free-function form of sslCLASS_REF::~sslCLASS_REF() (0x825234E0, re-sourced in
// sslCLASS_REF__dtor.cpp). Authored-def TU — no standalone binary function exists for this
// spelling; the bridge callers invoke the dtor directly.
#include "../../headers/ws/ssl/sslCLASS_REF.h"

void sslCLASS_REF_dtor(sslCLASS_REF *self)
{
    self->~sslCLASS_REF();
}
