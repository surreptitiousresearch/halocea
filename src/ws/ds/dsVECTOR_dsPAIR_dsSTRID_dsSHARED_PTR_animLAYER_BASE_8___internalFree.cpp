#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSHARED_PTR.h"
#include "Deleter.h"
#include "../anim/animLAYER_BASE.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsSTRID,dsSHARED_PTR<animLAYER_BASE,0,Deleter<animLAYER_BASE>>>,8>::_internalFree
// @ 0x8267A098
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > >, 8>::_internalFree(void *p)
{
    dlFree(p);
}
