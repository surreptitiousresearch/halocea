#pragma once
/* exposed_parameter — a runtime-tunable parameter registered with the exposed-parameter system. The
 * value union holds a pointer to the live storage, reinterpreted per the parameter's type (12 bytes).
 * DB models the pointer overlay as anonymous-union member ___u2 (_C48C822FD3E0506D4FCC38D528467D21);
 * $-name kept verbatim ($ in identifiers is an MSVC extension), matching the mtlPARAM.h convention. */

#include "parameter_type.h"   /* dedicated enum; was locally redefined as `typedef int parameter_type` */

/* DB union _C48C822FD3E0506D4FCC38D528467D21 — live-storage pointer overlay (4 bytes). */
typedef union _C48C822FD3E0506D4FCC38D528467D21
{
    void  *location;
    float *real_param;
    int   *integer_param;
} _C48C822FD3E0506D4FCC38D528467D21;

typedef struct exposed_parameter
{
    char          *parameter_name;   /* 0x00 */
    parameter_type type;             /* 0x04 */
    union _C48C822FD3E0506D4FCC38D528467D21 ___u2; /* 0x08 */
} exposed_parameter;                 /* 0x0C */

extern exposed_parameter g_exposed_params[64];
extern int               g_num_exposed_params;
