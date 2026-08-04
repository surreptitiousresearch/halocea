#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

// dsVECTOR<sslSTACK_DATA,8>::Clear @ 0x82AF0D10
// Destroy each live element's payload via sslSTACK_DATA::Cleanup (the destructor's real teardown
// logic, called directly here rather than through ~sslSTACK_DATA), then reset the count.
// CAVEAT: Cleanup() is declared `protected` on sslSTACK_DATA; the disassembly calls it directly
// from this specialization, so dsVECTOR<sslSTACK_DATA,8> must have friend access in the original
// source (not modeled as an explicit `friend` in the header — left as-is per the DB-verified call).
template<>
void dsVECTOR<sslSTACK_DATA, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].Cleanup();
    this->nElem = 0;
}
