#pragma once
/* file_reference — Blam file/path descriptor (location header magic at [0], drive/path id word at [6],
 * path/name payload following). Modeled as an opaque byte block at its DB size; callers touch fields by
 * offset, matching the engine's layout. */

typedef struct file_reference { char data[268]; } file_reference; /* 0x10C */
