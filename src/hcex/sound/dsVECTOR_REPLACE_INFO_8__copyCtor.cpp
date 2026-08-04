#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

// dsVECTOR<HALO_SOUND_PARAMS::REPLACE_INFO,8>::dsVECTOR(const dsVECTOR&) @ 0x836BDC84
// Copy-construct: start empty, adopt the source's allocation call-site cookie, then splice in a
// copy of the full source element range.
template<>
dsVECTOR<HALO_SOUND_PARAMS::REPLACE_INFO, 8>::dsVECTOR(
    const dsVECTOR<HALO_SOUND_PARAMS::REPLACE_INFO, 8> &src)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = src.__cl;
    this->Insert(0, src.pData, src.nElem);
}
