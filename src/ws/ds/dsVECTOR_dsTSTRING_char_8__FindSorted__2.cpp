#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending comparator

// dsVECTOR<dsTSTRING<char>,8>::FindSorted<dsTSTRING<char>> @ 0x82602C58
// Convenience overload: default-constructs a stateless dsCMP and delegates to
// FindSorted<dsCMP,dsTSTRING<char>>(key, cmp).
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::FindSorted<dsTSTRING<char>>(const dsTSTRING<char> &key) const
{
    dsCMP cmp;
    return this->FindSorted<dsCMP, dsTSTRING<char>>(key, cmp);
}
