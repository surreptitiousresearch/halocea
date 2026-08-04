#include "dsVECTOR.h"
#include "dsTSTRING.h"

// dsVECTOR<dsTSTRING<char>,8>::Reserve @ 0x823B6988
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
