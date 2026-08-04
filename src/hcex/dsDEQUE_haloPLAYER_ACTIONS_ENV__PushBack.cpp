#include <string.h>
#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::PushBack(const haloPLAYER_ACTIONS_ENV&) @ 0x823C0AE0
// Grow the ring by one at the back, copy `val` into the new tail slot, and return it.
template<>
haloPLAYER_ACTIONS_ENV &dsDEQUE<haloPLAYER_ACTIONS_ENV>::PushBack(const haloPLAYER_ACTIONS_ENV &val)
{
    this->ResizeBack(this->length + 1);
    haloPLAYER_ACTIONS_ENV *slot = this->Back();
    if (slot)
        memcpy(slot, &val, sizeof(haloPLAYER_ACTIONS_ENV));
    return *this->Back();
}
