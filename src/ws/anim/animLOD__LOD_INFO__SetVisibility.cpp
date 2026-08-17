#include "animLOD_LOD_INFO.h"
// DEVIATION: this TU previously carried a local 32-byte `struct objOBJ` stub (opaque head +
// stateProc at 0x1C), on the premise that ws/obj/objOBJ.h re-included a conflicting legacy-Blam
// animINST.h. That premise no longer holds — ws/anim/animINST.h forwards to the single canonical
// src/headers/animINST.h, which is exactly what ws/obj/objOBJ.h includes — and the stub was an ODR
// divergence against the real 220-byte (0xDC) definition. The canonical header is included instead.
// DB (types_members objOBJ): stateProc @ offset 28 (0x1C), size 4, `int` — the stub's one live
// member was at the right offset, so the generated access is unchanged.
#include "../../headers/ws/obj/objOBJ.h"

// ?SetVisibility@LOD_INFO@animLOD@@QAAX_N@Z  (@0x82B0DAF8) — show/hide every object in this LOD
// level by toggling bit 0 of each object's stateProc word.
void animLOD::LOD_INFO::SetVisibility(bool show)
{
    int hiddenBit = !show; // bit 0 is set when the level is hidden
    for (int i = 0; i < this->objs.nElem; ++i)
    {
        objOBJ *obj = this->objs[i];
        obj->stateProc = (obj->stateProc & ~0x1u) | hiddenBit;
    }
}
