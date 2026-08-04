#include "../../headers/ws/prop/propHALO_OBJ.h"

// propHALO_OBJ::~propHALO_OBJ() @ 0x823EC340 (thunk)
// propHALO_OBJ adds only POD float fields over propBASE, so its own destructor is a plain
// thunk straight through to the base -- no vtable reset or extra teardown of its own.
propHALO_OBJ::~propHALO_OBJ()
{
    propBASE::~propBASE();
}
