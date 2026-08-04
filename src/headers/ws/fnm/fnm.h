#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine fnm (filename utility) subsystem -- free functions operating on raw C-string
// paths/names/extensions with a fixed-size caller-supplied output buffer. Legacy Blam-style
// (cseries fileio) API, exposed to the ws engine under the fnm* namespace.
// See also fnmDIR.h (cached directory-tree search) and fnm_boundary.h (external helpers).

// 0x82625098 (?fnmAddExt@@YAPADPBD0PAD@Z) -- copy `name` into outBuffer, then either replace its
// existing extension (found after the last path delimiter) with `ext`, strip the extension
// (`ext` == nullptr), or append `.ext` if `name` had none. Returns outBuffer.
char *fnmAddExt(const char *name, const char *ext, char *outBuffer);

// 0x82624FD0 (?fnmAddPath@@YAPADPBD0PAD@Z) -- copy `path` into outBuffer, ensure it ends with a
// single '\\' path delimiter, then append `name`. Returns outBuffer.
char *fnmAddPath(const char *path, const char *name, char *outBuffer);

// 0x82626040 (?fnmBuildAbsolutePath@@YAPBDPADPBD@Z) -- if `name` already looks like a drive-letter
// absolute path ("d:..."), copy it into dst as-is (bounded to 260 chars); otherwise prefix it with
// "D:\\". Returns dst.
const char *fnmBuildAbsolutePath(char *dst, const char *name);

// 0x82626490 (?fnmResolvePath@@YAPBDPADPBD@Z) -- if `name` starts with "./" or ".\\", seed dst
// with the process startup directory first; then delegate to fnmResolvePathWithGivenBase to
// splice `name` (minus a leading "./"/".\\") onto dst, or copy `name` verbatim into dst when it
// doesn't start with "./"/".\\". Returns dst.
const char *fnmResolvePath(char *dst, const char *name);

// 0x82626268 (?fnmResolvePathWithGivenBase@@YAPBDPADPBD@Z) -- if `name` starts with "./" or ".\\",
// append (name+2) onto the existing contents of dst (adding a '/' delimiter first if dst doesn't
// already end with one); otherwise overwrite dst with `name` verbatim. Asserts the result is under
// 260 chars unless IGNORE_STRONG_ASSERT. Returns dst.
char *fnmResolvePathWithGivenBase(char *dst, const char *name);

// 0x82625580 (?fnmGetName@@YAPADPBDPAD@Z) -- copy `name`'s filename (last path component) with
// its extension stripped into outBuffer; returns outBuffer. Delegates to fnmGetNameExt then
// truncates at the last '.'.
char *fnmGetName(const char *name, char *outBuffer);

// 0x82625460 (?fnmGetNameExt@@YAPADPBDPAD@Z) -- copy `name`'s filename WITH extension (last path
// component) into outBuffer; returns outBuffer. Body external to this batch. // boundary
char *fnmGetNameExt(const char *name, char *outBuffer);

// 0x823C5E08 (?fnmGetNameStr@@YA?AV?$dsTSTRING@D@@ABV1@@Z) -- dsTSTRING form of fnmGetName:
// returns `name`'s filename (no path, no extension) as a fresh dsTSTRING<char>.
dsTSTRING<char> fnmGetNameStr(const dsTSTRING<char> &name);

// 0x823C5C78 (?fnmGetNameExtStr@@YA?AV?$dsTSTRING@D@@ABV1@@Z) -- dsTSTRING form of fnmGetNameExt:
// returns `name`'s filename (no path, extension kept) as a fresh dsTSTRING<char>.
dsTSTRING<char> fnmGetNameExtStr(const dsTSTRING<char> &name);

// 0x828F6E50 (?fnmFindDisc@@YAPADPBD00PAD@Z) -- search the cached directory trees rooted at
// fnmDirList[0..fnmNDir) for the directory tree node matching `path_` (via fnmDIR::FindSubDir),
// then look up `name`(.`ext`) inside it (via fnmDIR::FindName). Asserts strlen(name) < 128 unless
// IGNORE_STRONG_ASSERT. Returns outBuffer on a hit, or nullptr.
char *fnmFindDisc(const char *path_, const char *name, const char *ext, char *outBuffer);
