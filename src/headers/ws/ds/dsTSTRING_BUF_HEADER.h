#pragma once
// ws-engine ds: ref-counted heap buffer backing a dsTSTRING<T>.
// DB-verified layout (types_members dsTSTRING_BUF_HEADER<char>):
//   refCount@0, strLen@4, maxStrLen@8, str@12 (T[1] flexible tail) — size 16.

template<class T>
struct dsTSTRING_BUF_HEADER {
    int refCount;   // 0x00
    int strLen;     // 0x04
    int maxStrLen;  // 0x08
    T   str[1];     // 0x0C flexible array — the character data follows the header
};
