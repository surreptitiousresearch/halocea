// dsPAIR<int,HCEX_LIGHT> &dsPAIR<int,HCEX_LIGHT>::operator=(const dsPAIR<int,HCEX_LIGHT> &that)
//     @ 0x823D7C68
//
// Copy-assign one (id -> HCEX_LIGHT) map entry: copy the `int` key, then assign the value via
// HCEX_LIGHT::operator= (copy id; if the weak-entity handle isn't already shared, release this->
// ent's handle and adopt that.ent's, ref-bumping if non-null; copy entLight). The disassembly shows
// HCEX_LIGHT::operator= inlined on this->val; delegating here reproduces it exactly. The key copy is
// unconditional and the ent-rebind's self-guard lives inside HCEX_LIGHT::operator=, matching the
// disasm. Reversed per-instantiation.

#include "../headers/hcex/hcex_light.h" // HCEX_LIGHT, dsPAIR<int,HCEX_LIGHT> (hcex_light_pair)

template<>
dsPAIR<int, HCEX_LIGHT> &dsPAIR<int, HCEX_LIGHT>::operator=(const dsPAIR<int, HCEX_LIGHT> &that)
{
    this->key = that.key;
    this->val = that.val;
    return *this;
}
