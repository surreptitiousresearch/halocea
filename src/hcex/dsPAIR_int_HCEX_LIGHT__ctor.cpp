// dsPAIR<int,HCEX_LIGHT>::dsPAIR(const int &k, const HCEX_LIGHT &v) @ 0x823D7CD8
//
// The (key, value) entry ctor for a (id -> HCEX_LIGHT) map. Body is the inline two-copy ctor defined
// on the dsPAIR<K,V> primary template (dsPAIR.h): key(k), val(v). What the decompiler shows as an
// explicit body is that ctor with its member copy-ctors inlined:
//   * key(k) -> plain int copy (this->key = *k).
//   * val(v) -> HCEX_LIGHT copy ctor: copy id, adopt v.ent's weak handle (osLockedIncrement if
//               non-null), copy entLight.
// No distinct out-of-line source exists — this instantiation is generated from that template. This
// TU exists only to record the mapping from the 0x823D7CD8 address to the template ctor.

#include "../headers/hcex/hcex_light.h" // HCEX_LIGHT, dsPAIR<int,HCEX_LIGHT> (hcex_light_pair)
