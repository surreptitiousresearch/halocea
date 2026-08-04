#pragma once
/* Win32/Xenon SEH types — XDK boundary, kept opaque except for the one field this codebase reads. */

typedef struct _EXCEPTION_RECORD _EXCEPTION_RECORD;
typedef struct _CONTEXT _CONTEXT;

typedef struct _EXCEPTION_POINTERS
{
    _EXCEPTION_RECORD *ExceptionRecord; /* 0x00 */
    _CONTEXT           *ContextRecord;  /* 0x04 */
} _EXCEPTION_POINTERS;
