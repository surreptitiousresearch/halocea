// dsPAIR<unsigned long, HCEX_MODEL>::dsPAIR(const dsPAIR<unsigned long, HCEX_MODEL> &that) @ 0x823E7000
//
// Copy-construct one model-registry entry: trivially copy the unsigned long key, then deep-copy the
// HCEX_MODEL value via its copy ctor (shares the haloName/hcexName buffers ref-bumped and
// Insert-copies objNames/nodeIndices/regions). Reversed per-instantiation.

#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/hcex/HCEX_MODEL.h"

template<>
dsPAIR<unsigned long, HCEX_MODEL>::dsPAIR(const dsPAIR<unsigned long, HCEX_MODEL> &that)
    : key(that.key), val(that.val)
{
}
