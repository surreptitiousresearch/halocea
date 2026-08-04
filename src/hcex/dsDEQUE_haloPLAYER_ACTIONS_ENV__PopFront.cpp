#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::PopFront() @ 0x823C0B78
// Drop the front element by shrinking the ring to length-1 from the head.
template<>
void dsDEQUE<haloPLAYER_ACTIONS_ENV>::PopFront()
{
    this->ResizeFront(this->length - 1);
}
