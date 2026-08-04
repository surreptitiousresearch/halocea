#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::~dsDEQUE() @ 0x823BF800
// Destroys the backing vector; the sole instruction is the (implicit) member call
// dsVECTOR<haloPLAYER_ACTIONS_ENV,8>::~dsVECTOR(&this->data), emitted automatically here.
template<>
dsDEQUE<haloPLAYER_ACTIONS_ENV>::~dsDEQUE()
{
}
