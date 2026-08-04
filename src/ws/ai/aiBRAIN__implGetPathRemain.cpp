// aiBRAIN::implGetPathRemain @ 0x832AF2D0
// ?implGetPathRemain@aiBRAIN@@UAAMXZ
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"

// Remaining length of the active PATH behaviour (best-behaviour id 4), or -1 when there is none.
// The path-length getter is a bhvPATH virtual reached at vtable byte offset 0xF8 (int arg 0); it is
// dispatched by raw slot here rather than fabricating the bhvPATH vtable layout.
float aiBRAIN::implGetPathRemain()
{
    void *pathBhv = mind.best.pointee->FindBHV_2(4);
    if (!pathBhv)
        return -1.0f;
    void **vtbl = *reinterpret_cast<void ***>(pathBhv);
    typedef float (*GetRemainFn)(void *, int);
    GetRemainFn getRemain = reinterpret_cast<GetRemainFn>(vtbl[0xF8 / sizeof(void *)]);
    return getRemain(pathBhv, 0);
}
