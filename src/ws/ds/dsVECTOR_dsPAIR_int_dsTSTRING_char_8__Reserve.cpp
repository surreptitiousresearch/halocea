#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::Reserve @ 0x825D01B8
// Grow the backing store to at least `size` elements via Realloc; no-op if already large enough.
template<>
void dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
