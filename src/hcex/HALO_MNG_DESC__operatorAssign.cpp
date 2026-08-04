#include "../headers/hcex/HALO_MNG_DESC.h"

// 0x823E6C04 — HALO_MNG_DESC has no fields of its own; copy-assign forwards entirely to the
// inherited iaDESC::operator=.
HALO_MNG_DESC &HALO_MNG_DESC::operator=(const HALO_MNG_DESC &that)
{
    iaDESC::operator=(that);
    return *this;
}
