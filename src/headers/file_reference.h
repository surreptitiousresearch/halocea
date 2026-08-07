#pragma once
/* file_reference — Blam file/path descriptor, 0x10C bytes (DB `file_reference` = char data[268]; the PDB
 * models the type itself as an opaque block). Its leading 0x108 bytes are the DB type file_reference_info
 * (signature/flags/location/path) and the trailing 4 bytes hold the open Win32 handle that file_open stores
 * and file_close clears (data[264]). Expressed as a union so the size and layout are unchanged (268 bytes,
 * matching both .sweep/db_struct_sizes.tsv and .sweep/header_sizes.tsv) while the descriptor fields are
 * reachable by name; every existing data[] accessor keeps working. */

#include "file_reference_info.h"

typedef union file_reference
{
    char                data[268];     /* 0x000 — whole block, incl. the handle at data[264] */
    file_reference_info info;          /* 0x000..0x107 — named descriptor view */
    /* The trailing handle is outside file_reference_info (which is exactly 0x108), so it needs an
     * arm of its own to be reachable by name. Added 2026-08-07: clang's -Wcast-align flagged 33
     * `*(void **)&file->data[264]`-style reads across 14 file_*.c TUs, and the named view for
     * offsets 4/6/8 already existed — the call sites had simply never been migrated to it. */
    struct
    {
        char  reserved[264];           /* 0x000..0x107 — the file_reference_info block */
        void *handle;                  /* 0x108 — Win32 HANDLE: set by file_open, cleared by file_close */
    } win32;
} file_reference;                       /* 0x10C = 268 bytes */
