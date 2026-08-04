#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/hcexCINE_INFO.h"

// dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO,8>>::dsPAIR(
//     const dsTSTRING<char> &k, const dsVECTOR<hcexCINE_INFO,8> &v) @ 0x823B8A28
//
// The (key, value) entry ctor for the cinematics list map (dsTSTRING name -> dsVECTOR<hcexCINE_INFO>
// per-actor snapshots; see hcexCINE_INFO.h / cbAddCine). Body is the inline two-copy ctor defined on
// the dsPAIR<K,V> primary template (dsPAIR.h): key(k), val(v). What the decompiler shows as an
// explicit body is that ctor with its member copy-ctors inlined:
//   * key(k)  -> dsTSTRING<char> copy ctor: adopt k's buffer pointer and ++refCount.
//   * val(v)  -> dsVECTOR<hcexCINE_INFO,8> copy ctor: null storage, adopt v.__cl, then
//                Insert(0, v.pData, v.nElem) to deep-copy the source range.
// No distinct out-of-line source exists — this instantiation is generated from that template.
