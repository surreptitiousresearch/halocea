#pragma once
/* shader — the common base header shared by every shader variant (specific shader types extend it). Layout
 * from the database (40 bytes): a single _shader base block. */

#include "_shader.h"

typedef struct shader
{
    _shader base;                   /* 0x00 — DB member name */
} shader;                            /* 0x28 */
