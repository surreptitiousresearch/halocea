#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSHARED_PTR.h"
#include "Deleter.h"
#include "../anim/animLAYER_BASE.h"

// dsVECTOR<dsPAIR<dsSTRID,dsSHARED_PTR<animLAYER_BASE,0,Deleter<animLAYER_BASE>>>,8>::Reserve
// @ 0x8267EAA0
// Grow the backing store to at least `size` elements via Realloc; no-op if already large enough.
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > >, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
