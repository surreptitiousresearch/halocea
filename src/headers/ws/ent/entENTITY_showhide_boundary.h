#pragma once
#include "entENTITY.h"
#include "entDESC.h"
// Boundary helpers for entENTITY::implHide/implShow/ShowToCamera/HideFromCamera/implSetMaterial.
// These are external to this batch (resolved as the next frontier).

// Desc<entENTITY>(self) -- fetch the entity's per-class descriptor (entDESC). In the binary this
// is the template accessor ?Desc@...; modeled here as a plain overload for source clarity.
entDESC *Desc(entENTITY *self);

// Shared empty-string / zero-byte global at 0x8200155A, used as a fallback for unnamed objOBJ
// nodes during the material-name scan.
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
