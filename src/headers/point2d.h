#pragma once
/* point2d — DB-confirmed union of the flat n[2] view and the named x/y view
 * (4 bytes). ($-name kept verbatim; $ in identifiers is an MSVC extension.) */

/* DB struct _13C78DD7EDF1CF1C4AE9B0D54E891AA2 — x/y view. */
typedef struct _13C78DD7EDF1CF1C4AE9B0D54E891AA2
{
    __int16 x; /* 0x00 */
    __int16 y; /* 0x02 */
} _13C78DD7EDF1CF1C4AE9B0D54E891AA2;

typedef union point2d
{
    __int16 n[2];                                  /* 0x00 */
    struct _13C78DD7EDF1CF1C4AE9B0D54E891AA2 __s1; /* 0x00 — x/y view */
    struct { __int16 x, y; };                      /* 0x00 — direct view (consumers use pt.x/pt.y) */
} point2d; /* 4 bytes */
