#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending/equality comparator

// dsVECTOR<dsTSTRING<char>,8>::Find<dsTSTRING<char>> @ 0x825B3670
// Convenience overload: default-constructs a stateless dsCMP and delegates to
// Find<dsCMP,dsTSTRING<char>>(key, cmp, 0).
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Find<dsTSTRING<char>>(const dsTSTRING<char> &key) const
{
    dsCMP cmp;
    return this->Find<dsCMP, dsTSTRING<char>>(key, cmp, 0);
}
