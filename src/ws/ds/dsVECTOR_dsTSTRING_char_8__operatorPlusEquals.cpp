#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::operator+= @ 0x826889D0
// CAVEAT: the decompiler flags this as a thunk. It is a real, tiny function that tail-calls
// PushBack(value) directly (no additional work) — reproduced faithfully as a one-line forward.
template<>
dsTSTRING<char> &dsVECTOR<dsTSTRING<char>, 8>::operator+=(const dsTSTRING<char> &value)
{
    return this->PushBack(value);
}
