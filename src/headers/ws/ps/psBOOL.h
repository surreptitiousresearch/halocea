#pragma once
// ws-engine ps: a 4-byte boolean value that also remembers which text style it was written in
// (e.g. "true"/"false" vs "yes"/"no" vs "on"/"off"), so CompileToString can round-trip the exact
// spelling. DB-verified layout (types_members psBOOL): data@0 (unsigned int) — size 4.

typedef struct psBOOL {
    // Byte 0 of `data` is the boolean value (0/1); byte 1 is the style
    // (0 = "false"/"true", 1 = "no"/"yes", 2 = "off"/"on").
    unsigned int data;          // 0x00 DB member name — raw packed value

    // 0x82A312B0 — classify the `len`-byte text at `str` as one of the recognized boolean spellings
    // ("no"/"yes"/"off"/"on"/"false"/"true", case-insensitive). On a match, writes the boolean value
    // into byte 0 and the matched style (0/1/2, see above) into byte 1 of `*data` (when non-null)
    // and returns 1; returns 0 on no match (leaving `*data` untouched). Static.
    static int IsThisBoolString(const char *str, int len, unsigned int *data);
} psBOOL;
