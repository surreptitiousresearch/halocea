#include <stdint.h>
#include "headers/exception_pointers.h"

typedef struct _iobuf _iobuf;

typedef struct _CONTEXT _CONTEXT;
extern void stack_walk_with_context(struct _iobuf *error_stream, int16_t levels_to_ignore, _CONTEXT *context_pointer);

int generic_exception_filter(unsigned int exception_code, _EXCEPTION_POINTERS *exception_information)
{
    stack_walk_with_context(0, 0, exception_information->ContextRecord);
    return 1;
}
