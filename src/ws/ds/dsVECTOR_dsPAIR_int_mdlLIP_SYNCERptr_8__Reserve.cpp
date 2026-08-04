#include "dsVECTOR.h"
#include "dsPAIR.h"

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::Reserve @ 0x823E06B0
// Grow the backing store to at least `size` elements via Realloc; no-op if already large enough.
template<>
void dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
