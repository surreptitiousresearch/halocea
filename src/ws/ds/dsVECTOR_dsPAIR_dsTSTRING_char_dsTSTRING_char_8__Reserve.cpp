#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::Reserve @ 0x823E0288
// Grow the backing store to at least `size` elements via Realloc; no-op if already large enough.
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
