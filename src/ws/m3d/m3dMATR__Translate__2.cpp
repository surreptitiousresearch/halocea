#include "m3dMATR.h"

// m3dMATR::Translate(const m3dV*, int) @ 0x82542AA8  (overload __2 — higher address)
// Thin forwarder to the (float,float,float,int) base overload (m3dMATR__Translate.cpp).
// DEVIATION: the decompiler rendered the 4th (int) argument as (int)v; the disassembly
// shows r5 (the incoming `oper` param) moved to r7 and forwarded — the real argument is
// `oper`, not a re-cast of the pointer.
void m3dMATR::Translate(const m3dV *v, int oper)
{
    Translate(v->x, v->y, v->z, oper);
}
