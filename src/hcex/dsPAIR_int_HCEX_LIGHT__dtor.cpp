// dsPAIR<int,HCEX_LIGHT>::~dsPAIR() @ 0x823D6A20
//
// Destroy one (id -> HCEX_LIGHT) map entry. The key `int` is trivial; the value destructs via
// HCEX_LIGHT::~HCEX_LIGHT() (drop the weak-entity handle's reference, operator-delete it at zero,
// null the handle). The disassembly shows that dtor inlined on this->val.ent.pHandle; calling the
// member destructor here reproduces it exactly. Reversed per-instantiation.

#include "../headers/hcex/hcex_light.h" // HCEX_LIGHT, dsPAIR<int,HCEX_LIGHT> (hcex_light_pair)

template<>
dsPAIR<int, HCEX_LIGHT>::~dsPAIR()
{
    this->val.~HCEX_LIGHT();
}
