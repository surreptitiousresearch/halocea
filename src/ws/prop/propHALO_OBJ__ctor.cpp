#include "../../headers/ws/prop/propHALO_OBJ.h"

// propHALO_OBJ::propHALO_OBJ() @ 0x823EC2D8
// Base-construct, install this class's vtable, zero timeTerm, and default every funcOut/funcIn
// channel to 1.0.
//
// DEVIATION: the decompiler rendered the funcOut/funcIn init as a confusing pointer-walk starting
// from &funcOut[3] with HIDWORD-style pre/post-decrement arithmetic ("local variable allocation
// has failed" class of artifact). The disassembly is unambiguous: `r11 = this+0x38` (funcOut[3]);
// each of 4 iterations stores 1.0 at `r11-0xC` (funcOut[loop index]) then bumps r11 by 4 and
// stores 1.0 there too (funcIn[loop index]) -- i.e. a plain interleaved fill of both 4-float
// arrays with 1.0, reconstructed below as two ordinary loops.
propHALO_OBJ::propHALO_OBJ() : propENT() // immediate base (chains to propBASE)
{
    this->__vftable = (propBASE_vtbl *)&propHALO_OBJ::vftable;
    this->timeTerm = 0.0f;
    for (int i = 0; i < 4; ++i)
    {
        this->funcOut[i] = 1.0f;
        this->funcIn[i] = 1.0f;
    }
}
