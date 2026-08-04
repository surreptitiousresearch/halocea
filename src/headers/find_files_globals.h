#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* find_files_globals @ find_files_globals — recursive directory-walk state (find_files.c).
 * DB layout (616 bytes). `data` is the Win32 WIN32_FIND_DATAA scratch for the current handle. */

#include "win32_find_data.h"

typedef struct find_files_globals
{
    unsigned int     flags;          /* 0x000 */
    __int16          depth;          /* 0x004 — current recursion depth, -1 when idle */
    __int16          location;       /* 0x006 — drive/path id */
    char             path[256];      /* 0x008 */
    void            *handles[8];     /* 0x108 — per-depth FindFirstFile handles */
    WIN32_FIND_DATAA data;           /* 0x128 — WIN32_FIND_DATAA */
} find_files_globals_t;              /* 616 bytes */

extern find_files_globals_t find_files_globals;
