#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/hcex/hcexJOB_SYNC_INST.h"
#include "../../headers/ws/ds/ds_new.h"

// dsCONST_ARRAY<hcexJOB_SYNC_INST,4>::dsCONST_ARRAY(int) @ 0x823D1878
// ??0?$dsCONST_ARRAY@VhcexJOB_SYNC_INST@@$03@@QAA@H@Z
// Count-taking ctor: store the live-entry count, then default-construct all N=4 inline element
// slots. DEVIATION: the decompiler flattened this into a single garbled body (it inlined the
// hcexJOB_SYNC_INST default ctor across the four elements, walking a mis-typed base pointer
// backwards in 1628-byte strides). The disassembly (li r29,3; loop; osCreateSignal x2 per pass)
// confirms the compiler emitted the standard array member-construct loop over all four slots.
template<>
dsCONST_ARRAY<hcexJOB_SYNC_INST, 4>::dsCONST_ARRAY(int nEntry)
{
    this->nEntry = nEntry;
    for (int i = 0; i < 4; ++i)
        new (&this->list[i]) hcexJOB_SYNC_INST();
}
