#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "dsSWAP.h"

struct dsCMP {}; // boundary — stateless default ordering, forwarded to the templated Sort

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::Sort @ 0x82AA5A60
// Non-templated overload: builds a default dsCMP and dsSWAP policy object and forwards to the
// templated Sort<dsCMP,dsSWAP<...>> (an extern-boundary sibling call, not re-reversed here).
template<>
void dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::Sort()
{
    dsCMP cmp = {};
    dsSWAP<dsPAIR<int, dsTSTRING<char> > > swap = {};
    this->Sort<dsCMP, dsSWAP<dsPAIR<int, dsTSTRING<char> > > >(cmp, swap);
}
