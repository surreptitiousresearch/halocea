#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::`scalar deleting destructor' @ 0x826244D0
// Compiler-generated deleting-destructor entry point (not the plain destructor): runs the
// destructor, then conditionally calls operator delete(this) when bit 0 of the flag argument is
// set (the "delete this too" case used when destroying through a virtual/deleting-dtor call).
template<>
dsVECTOR<dsTSTRING<char>, 8> *dsVECTOR<dsTSTRING<char>, 8>::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~dsVECTOR<dsTSTRING<char>, 8>();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
