#pragma once
#include "../fio/fio_boundary.h"
// fio_boundary.h supplies the shared STRONG_ASSERT_DUMMY::Crash(ctx, expr, file, line, msgCtx)
// stub and IGNORE_STRONG_ASSERT / g_assertContextByte globals used by the fnm asserts below --
// this is the same shape the fnm decompiles call (explicit-nullptr-context, empty_string tail
// arg), matching fio's convention rather than ds_assert_boundary.h's this-call form.
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
extern int      fnmNDir;       // boundary -- number of live entries in fnmDirList
extern fnmDIR  *fnmDirList[];  // boundary -- array of directory-tree roots

// CRT snprintf thunk, as named in the database. // boundary
extern int _snprintf_0(char *buf, unsigned int size, const char *fmt, ...);
