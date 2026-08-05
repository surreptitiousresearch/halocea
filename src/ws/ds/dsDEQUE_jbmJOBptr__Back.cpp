#include "dsDEQUE.h"
#include "ds_assert_boundary.h"

struct jbmJOB; // boundary — job-manager work item (element is a pointer)

// dsDEQUE<jbmJOB*>::Back @ 0x826EC068
// Reference to the last live element (the slot at ring index `tail`); assert non-empty first.
template<>
jbmJOB **dsDEQUE<jbmJOB *>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->length <= 0)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "Length() > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_deque.hpp",
            318,
            empty_string);
    return &this->data[this->tail];
}
