#include "../headers/hcex/haloENGINE_CONTROL.h"

// dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>::dsPAIR(
//     const int &k, void (haloENGINE_CONTROL::* const &v)(unsigned long, const dsPARAM_LIST &))
//     @ 0x823C1878
//
// The (key, value) entry ctor for haloENGINE_CONTROL::GetEventMap()'s
// dsVECTOR<dsPAIR<int, haloENGINE_CONTROL-event-method-ptr>, 8> (see haloENGINE_CONTROL.h). Body is
// the inline two-copy ctor defined on the dsPAIR<K,V> primary template (dsPAIR.h): key(k), val(v).
// No distinct out-of-line source exists — this instantiation is generated from that template.
