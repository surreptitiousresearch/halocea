#pragma once
/* edit_text — text-edit control state (DB layout). The editable buffer pointer, its capacity, and the
 * cursor/selection indices touched by the edit helpers, at their true offsets. */

typedef struct /* align(4) */ edit_text
{
    char  *buffer;                  /* 0x00 */
    short  maximum_length;          /* 0x04 */
    short  insertion_point_index;   /* 0x06 */
    short  selection_start_index;   /* 0x08 */
    char   _pad0A[2];               /* 0x0A */
} edit_text; /* 12 bytes */
