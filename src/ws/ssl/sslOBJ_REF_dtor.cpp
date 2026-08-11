// Flattened C-linkage-style bridge for the hcex boundary headers (hcex_cine_init_boundary.h /
// hcex_obj_sync_game_state_boundary.h): free-function form of sslOBJ_REF::~sslOBJ_REF()
// (@0x825236C8, re-sourced in sslOBJ_REF__dtor.cpp). Authored-def TU — no standalone binary
// function exists for this spelling; the bridge callers invoke the dtor directly.
#include "../../headers/ws/ssl/sslOBJ_REF.h"

void sslOBJ_REF_dtor(sslOBJ_REF *self)
{
    self->~sslOBJ_REF();
}
