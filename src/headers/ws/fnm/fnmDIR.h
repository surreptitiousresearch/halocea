#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine fnm (filename/directory) subsystem: fnmDIR is a node in the cached directory tree
// built by fnmDirInit / fnmTryLoadContentFromCache. Each node holds the names of the files
// directly inside it (nameList) plus links to its first child directory (subDir) and next
// sibling directory (nextDir) -- a classic first-child/next-sibling n-ary tree.
// DB-verified layout (types_members fnmDIR): path@0, nameList@4 (dsVECTOR<char*,8>, size 20),
// nextDir@24, subDir@28 -- size 32.

typedef struct fnmDIR {
    char             *path;     // 0x00 this directory's full path
    dsVECTOR<char *,8> nameList; // 0x04 file names directly inside this directory (sorted)
    fnmDIR           *nextDir;  // 0x18 next sibling directory
    fnmDIR           *subDir;   // 0x1C first child directory

    // 0x828F3BB0 -- stateless comparator functor: case-insensitive compare of two raw names
    // (used to binary-search nameList when the caller already built the exact "name.ext" string).
    // Body external to this batch. // boundary
    struct nameListCMP {
        int cmp(const char *a, const char *b) const; // boundary
    };

    // 0x828F3BC0 -- stateless comparator functor: resolve `a`'s own name+ext via fnmGetName, then
    // case-insensitive compare against `b` (used when the caller only has a bare name, no ext).
    // Body external to this batch. // boundary
    struct fnmGetNameCMP {
        int cmp(const char *a, const char *b) const; // boundary
    };

    // 0x828F65E0 -- search this directory (and, on miss, recursively its subdirectories and
    // sibling directories) for a file. When `ext` is given, `name_ext` (the pre-built "name.ext")
    // is looked up directly via nameListCMP; otherwise `name` alone is looked up via
    // fnmGetNameCMP (which strips extensions off nameList entries before comparing). On a hit,
    // writes this directory's path + the matched name into outBuffer and returns it; returns
    // nullptr if nothing matches anywhere in the subtree.
    char *FindName(const char *name, const char *ext, const char *name_ext, char *outBuffer);

    // 0x828F3FF8 -- search this directory (and, on miss, recursively its subdirectories and
    // sibling directories) for a subdirectory whose full path matches `_path` (case-insensitive),
    // or whose path is a prefix of `_path` ending exactly at a path delimiter (i.e. `_path` names
    // a location inside this directory). Returns the matching fnmDIR node, or nullptr.
    fnmDIR *FindSubDir(const char *_path);
} fnmDIR;
