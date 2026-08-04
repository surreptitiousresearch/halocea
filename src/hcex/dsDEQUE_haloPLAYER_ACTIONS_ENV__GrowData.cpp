#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::GrowData(int minSize) @ 0x823C0888
// Ensure the backing vector holds at least minSize slots; grow to 3*minSize/2 and Reconstruct the
// ring when it must.
template<>
void dsDEQUE<haloPLAYER_ACTIONS_ENV>::GrowData(int minSize)
{
    if (minSize > this->data.nElem)
    {
        int oldSize = this->data.nElem;
        this->data.Resize(3 * minSize / 2);
        this->Reconstruct(oldSize);
    }
}
