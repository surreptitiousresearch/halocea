// dsPAIR<unsigned long, HCEX_MODEL>::dsPAIR(const unsigned long &k, const HCEX_MODEL &v) @ 0x823E69C8
//
// The (key, value) entry ctor for the model registry (modelId -> HCEX_MODEL). Body is the inline
// two-copy ctor defined on the dsPAIR<K,V> primary template (dsPAIR.h): key(k), val(v). The
// decompiler renders it as:
//     this->key = *k;                       // key(k): unsigned long, trivial copy
//     HCEX_MODEL::HCEX_MODEL(&this->val, v) // val(v): HCEX_MODEL copy ctor (out-of-line CALL)
// which is exactly that template ctor with the trivial key copy inlined and the non-trivial
// HCEX_MODEL member copy ctor left as a call. No distinct out-of-line source exists — this
// instantiation is generated from that template (see dsPAIR.h / HCEX_MODEL.h). Marker only.

#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/hcex/HCEX_MODEL.h"
