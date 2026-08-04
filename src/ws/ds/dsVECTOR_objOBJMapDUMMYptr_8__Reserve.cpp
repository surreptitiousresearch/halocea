#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsVECTOR<...objOBJ-map DUMMY*,8>::Reserve @ 0x82686AD0
template<>
void dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
