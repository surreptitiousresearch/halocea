#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)
struct dsCMP {};   // boundary — pointer-value comparator (empty; comparison is inlined)

// dsVECTOR<camCAMERA*,8>::Find<camCAMERA const*> @ 0x82703D10
// Convenience overload: default-constructs the stateless dsCMP comparator and delegates to the
// (cmp, startPos) form starting from index 0.
template<>
template<>
int dsVECTOR<camCAMERA *, 8>::Find<camCAMERA const *>(camCAMERA const *const &key) const
{
    dsCMP cmp;
    return this->Find<dsCMP, camCAMERA const *>(key, cmp, 0);
}
