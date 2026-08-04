#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbSetTimer(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324FBF0
// ?cbSetTimer@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetTimer" callback: read (float duration = argv[0], bool isCycle = argv[1]) and forward to the
// virtual implSetTimer, storing its int timer-id result into `retVal`. `caller` is passed by value
// and its ref is released on return. (Disasm at 0x8324FC34-0x8324FC44 confirms both the bool and the
// float reach implSetTimer through vtable slot 0x2D0 — the decompiler's single-arg cast is wrong.)
void aiBRAIN::cbSetTimer(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool isCycle = false;
    argv[1].GetValue<bool>(isCycle, DSD_CONV_RETRIEVE);
    float duration = 0.0f;
    argv[0].GetValue<float>(duration, DSD_CONV_RETRIEVE);

    int result = implSetTimer(duration, isCycle);
    retVal.StoreValue<int>(result);
    // the caller ref releases itself at scope end.
}
