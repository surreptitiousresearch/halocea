#pragma once
#include "dsTSTRING_BUF_HEADER.h"
// ws-engine ds: ref-counted string wrapper — a single pointer to a shared buffer header.
// DB-verified layout (types_members dsTSTRING<char>): pBuffer@0 — size 4.

struct apPROCESSOR_LOCK; // boundary — Saber processor-affinity debug guard

template<class T> struct dsTSTRING;
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &, const dsTSTRING<T> &);

template<class T>
struct dsTSTRING {
    dsTSTRING_BUF_HEADER<T> *pBuffer; // 0x00

    // operator+ concatenation uses the protected grow-and-shift Insert overload.
    friend dsTSTRING<T> operator+ <>(const dsTSTRING<T> &, const dsTSTRING<T> &);

    // Per-type processor-affinity guard asserted on Lock/Unlock (static; defined out of line).
    static apPROCESSOR_LOCK processorLock;

    // 0x823B0930 (dsTSTRING<char>) — pointer to the character data in the buffer header.
    const T *CStr() const;

    // Release this string's buffer reference, freeing it at zero. Inferred from the refcount-release
    // tail inlined at the end of every dsAFFIX_STRING/dsAFFIX_BUILDER ctor that holds a temporary
    // dsTSTRING<char>; body not decompiled as its own addressed function in this batch. // boundary
    ~dsTSTRING(); // boundary

    // Copy-assign: share `other`'s buffer (drop the old one). Body external to this batch.
    dsTSTRING<T> &operator=(const dsTSTRING<T> &other); // boundary

    // Default ctor -- adopts the shared empty-string singleton. boundary.
    dsTSTRING();

    // Construct from a NUL-terminated C string (ctor body is UnsafeInit(s, -1, 0)). boundary.
    dsTSTRING(const T *s);

    // Append another string to this one. Body external to this batch. // boundary
    dsTSTRING<T> &operator+=(const dsTSTRING<T> &other); // boundary

    // Character-wise equality with another string. // boundary
    bool operator==(const dsTSTRING<T> &other) const;

    // Read-only indexed character access (bounds not asserted here). Body not decompiled in
    // this batch -- referenced as a boundary call from osFileOpen. // boundary
    T operator[](int idx) const;

    // 0x823B0420 (dsTSTRING<char>) — initialise this (uninitialised) string from `s`: measure up to
    // `len` chars (len==-1 = whole string, stopping at the first NUL). When the result would be empty
    // (null `s`, or empty text with no reserve), adopt the shared empty-string singleton via
    // UnsafeInitEmpty. Otherwise allocate a private buffer with `reserve` extra capacity and copy the
    // measured characters in. Protected (IAA mangle); named `reserve` here (the extra capacity added
    // beyond the measured length), not an allocation-policy flag.
    void UnsafeInit(const T *s, int len, int reserve);

    // 0x823B0A00 (dsTSTRING<char>) — test whether the substring starting at character `ofs`
    // begins with the NUL-terminated `s` (optionally case-insensitive). An empty `s` matches.
    bool IsStartWith(const T *s, bool noCase, int ofs) const;

    // Test whether this string ends with the NUL-terminated `s` (optionally case-insensitive).
    // Referenced as a boundary call from hcex_is_breakable_surface (checks the "|h" suffix). boundary.
    bool IsEndWith(const T *s, bool noCase) const;

    // 0x824F7F90 (dsTSTRING<char>) — find the first occurrence of the NUL-terminated substring `str`
    // at or after character `start` (optionally case-insensitive). Returns the character index of
    // the match, or -1 if `start` is out of range, `str` is null, or there is no match. const (QBA).
    int Find(const T *str, int start, int noCase) const;

    // Find the LAST occurrence of the NUL-terminated substring `str` searching backward from
    // `start` (-1 = end of string), optionally case-insensitive. Returns the match character
    // index, or -1. const (QBA). boundary.
    int RFind(const T *str, int start, int noCase) const;

    // 0x82517E08 (dsTSTRING<char>) — writable pointer to character `idx`; forces a private
    // (unshared) buffer copy first if the buffer is shared.
    T *At(int idx);

    // 0x823C3AC0 (dsTSTRING<char>) — take a private, writable buffer of at least `len` chars and
    // return a pointer to its character data; pair with Unlock. Forces a unique (unshared) buffer,
    // growing/copy-on-write as needed, truncates to `len` chars, and NUL-terminates at `len`.
    T *Lock(int len);
    void Unlock();     // boundary

    // 0x823B12B0 (dsTSTRING<char>) — insert another string at a character position.
    dsTSTRING<T> &Insert(int pos, const dsTSTRING<T> &str);
    // 0x824F9D80 (dsTSTRING<char>) — insert `count` copies of character `c` at position `pos`.
    dsTSTRING<T> &Insert(int pos, T c, int count);
    // 0x823B4D80 (dsTSTRING<char>) — insert up to `len` characters of the NUL-terminated `s`
    // (len==-1 = whole string) at position `pos`; stops at the first NUL in `s`. Returns *this.
    dsTSTRING<T> &Insert(int pos, const T *s, int len);

    // 0x82520E58 (dsTSTRING<char>) — ensure the buffer can hold at least `strLen` characters,
    // growing it if needed. Reuses the buffer in place when uniquely owned (refCount==1),
    // otherwise copy-on-write into a fresh buffer and drop the old reference.
    void Reserve(int strLen);

    // Point this string at the process-wide shared empty-string singleton (lazily allocated
    // on first use, its refCount bumped). Does not release any prior buffer. // boundary
    void UnsafeInitEmpty();

    // 0x823D5A38 (dsTSTRING<char>) — reset to an empty string. If the buffer is uniquely owned,
    // truncate it in place; otherwise drop a reference and adopt the shared empty singleton.
    // Returns *this.
    dsTSTRING<T> &Clear();

    // 0x82517F90 (dsTSTRING<char>) — remove `len` characters starting at `pos` (len==-1 = to end).
    // Copy-on-write when the buffer is shared. Returns *this.
    dsTSTRING<T> &Erase(int pos, int len);

    // 0x824FADD0 (dsTSTRING<char>) — the substring of `len` characters from `pos` (len==-1 = to end),
    // returned by value (sret). A zero-length result adopts the shared empty-string singleton.
    dsTSTRING<T> SubStr(int pos, int len) const;

    // 0x823C3C50 (dsTSTRING<char>::ToLower, ?ToLower@?$dsTSTRING@D@@QAAXXZ) — lowercase this string's
    // characters in place (copy-on-write when the buffer is shared). Body external to this batch.
    void ToLower(); // boundary

    // 0x823C67C0 (dsTSTRING<char>::Lower, ?Lower@?$dsTSTRING@D@@QBA?AV1@XZ) — a lowercased copy of this
    // string, returned by value (sret). const (QBA). Body external to this batch.
    dsTSTRING<T> Lower() const; // boundary

    // dsAFFIX_BUILDER builds output strings via the protected grow-and-shift Insert(pos,len) gap
    // opener directly (as the original code does); grant it access without widening the public API.
    friend struct dsAFFIX_BUILDER;
    // psSYSTEM::LoadPsFromFile annotates errorDesc via the protected gap-opener directly (as the
    // original code does); grant access without widening the public API.
    friend struct psSYSTEM;
    // STRONG_ASSERT2_HELPER::asserd builds its message string via the protected AllocBuffer/Insert
    // helpers directly (as the original code does); grant it access.
    friend struct STRONG_ASSERT2_HELPER;

protected:
    // 0x823B0F78 (dsTSTRING<char>) — open a `len`-character gap at position `pos` (grow-and-shift):
    // grow the buffer to hold strLen+len chars, then shift the tail up past the gap. Mangle marks it
    // protected returning void (not a dsTSTRING& as an earlier boundary guess assumed). Used by the
    // public Insert overloads.
    void Insert(int pos, int len);

    // 0x823B00B8 (dsTSTRING<char>) — drop a reference on a buffer header, freeing at zero.
    void ReleaseBuffer(dsTSTRING_BUF_HEADER<T> *pBuf);

    // 0x823B0138 (dsTSTRING<char>) — allocate a fresh buffer header sized for `len` chars
    // (capacity at least `maxLen`), NUL-terminated, refCount 1. Does not touch this->pBuffer.
    dsTSTRING_BUF_HEADER<T> *UnsafeAllocBuffer(int maxLen, int len);

    // 0x823B0240 (dsTSTRING<char>) — allocate a fresh buffer header (capacity >= maxLen,
    // holding `len` chars, NUL-terminated, refCount 1).
    // Public: called directly by the strong-assert machinery (STRONG_ASSERT_DUMMY::Crash), a free
    // function that cannot be befriended without colliding with the struct model in other TUs. (2026-07-31)
public:
    dsTSTRING_BUF_HEADER<T> *AllocBuffer(int maxLen, int len);
protected:

    // 0x823B0CF0 (dsTSTRING<char>) — resize an existing buffer header in place to hold
    // `len` chars (capacity >= maxLen), NUL-terminated, refCount reset to 1.
    dsTSTRING_BUF_HEADER<T> *ReallocBuffer(dsTSTRING_BUF_HEADER<T> *pBuf, int maxLen, int len);
};

// Free concatenation operator (??$?HD@@YA?AV?$dsTSTRING@D@@ABV0@0@Z) @ 0x823B5448 (char) --
// returns a fresh string holding lhs+rhs by value.
template<typename T>
dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const dsTSTRING<T> &rhs);

// Free concatenation operator with a NUL-terminated C-string rhs
// (??$?HD@@YA?AV?$dsTSTRING@D@@ABV0@PBD@Z) @ 0x823B5518 (char) -- returns lhs+rhs by value; a null
// `rhs` is treated as the empty string.
template<typename T>
dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const T *rhs);

// Free concatenation operator with a NUL-terminated C-string lhs
// (??$?HD@@YA?AV?$dsTSTRING@D@@PBDABV0@@Z) -- returns lhs+rhs by value; a null `lhs` is treated as
// the empty string. Reversed use: aiDEBUG::AppendStateParamList (" = " + value).
template<typename T>
dsTSTRING<T> operator+(const T *lhs, const dsTSTRING<T> &rhs);
