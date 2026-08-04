#include "animINST.h"

// animINST::IsValid @ 0x825E1130
// True when NONE of the requested validity bits are currently pending in stateVld.
int animINST::IsValid(int state)
{
    return (this->stateVld & state) == 0;
}
