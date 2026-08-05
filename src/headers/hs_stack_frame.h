#pragma once
#include <stdint.h>
/* hs_stack_frame — one interpreter stack frame inside hs_thread.stack_data (DB-verified). */

typedef struct hs_stack_frame hs_stack_frame;
struct hs_stack_frame
{
    hs_stack_frame *parent;           /* 0x00 */
    int             expression_index; /* 0x04 */
    int            *child_result;     /* 0x08 */
    int16_t         size;             /* 0x0C */
    unsigned char   data[];           /* 0x0E */
};
