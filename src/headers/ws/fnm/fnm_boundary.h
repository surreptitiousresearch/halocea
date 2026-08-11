#pragma once
#include "../fio/fio_boundary.h"
// fio_boundary.h supplies STRONG_ASSERT_DUMMY (whose one real Crash is the 4-arg member at
// 0x825202A8) and the IGNORE_STRONG_ASSERT / empty_string globals the fnm asserts below use.
// The fnm decompiles call it exactly as every other subsystem does: through a null this
// pointer, with empty_string as the trailing `info` argument.
// Boundary declarations for helpers called by the fnm (filename/directory) subsystem functions
// but external to this batch.

struct fnmDIR; // forward decl (see fnmDIR.h) -- kept opaque here to avoid a hard include

// 0x82762A00-ish (?fnmGetName@@YAPADPBDPAD@Z) -- copy `name`'s file name (no path, no extension)
// into outBuffer; returns outBuffer. Body external to this batch. // boundary
char *fnmGetName(const char *name, char *outBuffer);

// (?fnmMakeFullName@@YAPADPBD00PADH@Z) -- join path+name+ext into outBuffer (flags in the
// trailing int control quoting/separator behaviour). Body external to this batch. // boundary
char *fnmMakeFullName(const char *path, const char *name, const char *ext, char *outBuffer, int flags);

// (?osGetStartupDir@@YAXPADH@Z) -- OS layer: copy the process startup directory into dst
// (size-bounded). Body external to this batch. // boundary
void osGetStartupDir(char *dst, int size);

// fnmDirInit-populated global directory-tree roots, one per registered search root
// (see fnmAddSearchPath / fnmGetNSearchPath). Walked by fnmFindDisc.
extern "C" int      fnmNDir;       // boundary -- number of live entries in fnmDirList
extern "C" fnmDIR  *fnmDirList[];  // boundary -- array of directory-tree roots

// CRT snprintf thunk, as named in the database. // boundary
extern "C" int _snprintf_0(char *buf, unsigned int size, const char *fmt, ...);
