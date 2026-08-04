#include <new> // placement operator new(size_t, void*)
#include "sslCLASS_REF.h"

// Global placement copy-construct helper: construct a copy of *val at *place, returning place
// (or null if place is null). Declared as the primary template it specializes.
template<class T> T *New(T *place, const T *val);

// New<sslCLASS_REF> @ 0x825C9ED8
// Placement copy-construct of an sslCLASS_REF: guards against a null placement slot (returns
// null in that case), otherwise copy-constructs in place and returns `place`.
// DEVIATION: decompiler typed this `void`, missing that the PPC ABI leaves the copy-ctor call's
// `this` (== place) live in r3 through the fallthrough path, which is what the caller actually
// consumes as the return value (verified against disasm_func: `li r3,0` on the null branch vs.
// falling through with r3 unchanged -- i.e. still `place` -- after the constructor call).
template<>
sslCLASS_REF *New<sslCLASS_REF>(sslCLASS_REF *place, const sslCLASS_REF *val)
{
    if (!place)
        return nullptr;

    new (place) sslCLASS_REF(*val);
    return place;
}
