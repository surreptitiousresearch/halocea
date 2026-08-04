#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending/equality comparator

// dsVECTOR<dsTSTRING<char>,8>::Find<char*> @ 0x82981890
// Convenience overload: default-constructs a stateless dsCMP and delegates to
// Find<dsCMP,char*>(key, cmp, 0).
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Find<char *>(char *const &key) const
{
    dsCMP cmp;
    return this->Find<dsCMP, char *>(key, cmp, 0);
}
