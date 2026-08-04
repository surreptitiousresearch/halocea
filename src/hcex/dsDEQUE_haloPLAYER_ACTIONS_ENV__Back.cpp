#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

extern int IGNORE_STRONG_ASSERT;   // ?IGNORE_STRONG_ASSERT@@3HA
extern const char empty_string;   // shared single-NUL empty-string constant

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::Back() @ 0x823C0148
// Pointer to the ring's tail element; strong-asserts the queue is non-empty first.
template<>
haloPLAYER_ACTIONS_ENV *dsDEQUE<haloPLAYER_ACTIONS_ENV>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->length <= 0)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "Length() > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_deque.hpp", 318, empty_string);
    return &this->data[this->tail];
}
