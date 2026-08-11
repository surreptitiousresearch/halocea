// FUNCTION_INDEX entry: dsDEQUE<haloPLAYER_ACTIONS_ENV>::Back() @0x823C0148 (?Back@?$dsDEQUE@UhaloPLAYER_ACTIONS_ENV@@@@QAAAAUhaloPLAYER_ACTIONS_ENV@@XZ)
#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::Back() @ 0x823C0148
// Pointer to the ring's tail element; strong-asserts the queue is non-empty first.
template<>
haloPLAYER_ACTIONS_ENV *dsDEQUE<haloPLAYER_ACTIONS_ENV>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->length <= 0)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("Length() > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_deque.hpp", 318, empty_string);
    return &this->data[this->tail];
}
