#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../scn/scnINST_CREATE_DATA.h"

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::Reserve @ 0x823E05D0
// Grow the backing store to at least `size` elements via Realloc; no-op if already large enough.
template<>
void dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
