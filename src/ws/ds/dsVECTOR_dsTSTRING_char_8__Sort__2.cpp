#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "dsSWAP.h"

struct dsCMP {}; // boundary — stateless ascending comparator

// dsVECTOR<dsTSTRING<char>,8>::Sort (no-arg) @ 0x82664B20
// Convenience overload: default-constructs a stateless dsCMP and dsSWAP<dsTSTRING<char>>, then
// delegates to Sort<dsCMP,dsSWAP<dsTSTRING<char>>>.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Sort()
{
    dsCMP cmp;
    dsSWAP<dsTSTRING<char>> swap;
    this->Sort<dsCMP, dsSWAP<dsTSTRING<char>>>(cmp, swap);
}
