#ifndef _LARGE_INTEGER_H
#define _LARGE_INTEGER_H

#include <stdint.h>

/* Win32 boundary type. DB-verified union (all members at offset 0, sizeof=8).
   Big-endian (PowerPC/Xbox 360): HighPart precedes LowPart.
   DB layout (types_members _LARGE_INTEGER):
     __s0     @0  (_BD17ED5EF6FA8CE7398C97BF41535960, 8 bytes — anonymous HighPart/LowPart struct)
     u        @0  (_LARGE_INTEGER::<unnamed_type_u>, 8 bytes — same HighPart/LowPart shape)
     QuadPart @0  (__int64)
   Type names kept verbatim from the DB per the mtlPARAM.h convention ($ in
   identifiers is an MSVC extension; <unnamed_type_u> is the IDA/mangler name
   for the anonymous inner struct). */

/* DB types_members _BD17ED5EF6FA8CE7398C97BF41535960 */
typedef struct _BD17ED5EF6FA8CE7398C97BF41535960
{
    int          HighPart;   /* 0x0 */
    unsigned int LowPart;    /* 0x4 */
} _BD17ED5EF6FA8CE7398C97BF41535960;

/* DB types_members _LARGE_INTEGER::<unnamed_type_u> — identical shape.
   The IDA/mangler placeholder `<unnamed_type_u>` is not a legal C identifier; renamed to a
   valid tag with the same layout. */
typedef struct _LARGE_INTEGER_u
{
    int          HighPart;   /* 0x0 */
    unsigned int LowPart;    /* 0x4 */
} _LARGE_INTEGER_u;

typedef union _LARGE_INTEGER
{
    _BD17ED5EF6FA8CE7398C97BF41535960 __s0; /* 0x0 — anonymous struct in the original source */
    _LARGE_INTEGER_u u;                     /* 0x0 */
    __int64 QuadPart;                       /* 0x0 */
} _LARGE_INTEGER; /* 8 bytes */

#endif /* _LARGE_INTEGER_H */
