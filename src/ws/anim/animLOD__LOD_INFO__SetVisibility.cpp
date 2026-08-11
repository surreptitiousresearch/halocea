#include "animLOD_LOD_INFO.h"

// objOBJ boundary — only the process-state word is needed here; the rest of the model object is
// opaque. Completes the forward declaration from animLOD_LOD_INFO.h without pulling the full obj
// header (which re-includes a conflicting Blam animINST.h). DB: objOBJ::stateProc@28.
struct objOBJ {
    unsigned char _opaque0[28]; // 0x00
    int           stateProc;    // 0x1C process-state flags (bit 0 = hidden)
};

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
