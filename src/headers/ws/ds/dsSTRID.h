#pragma once
// ws-engine ds: interned string id (a pooled/hashed const char*).
// DB-verified layout (types_members dsSTRID): id@0 (const char*) — size 4.

template<class T> struct dsTSTRING; // ds/dsTSTRING.h

struct dsSTRID {
    const char *id; // 0x00

    // 0x8252B898 — intern/hash `str`: if empty, adopt the pool's reserved empty-string id;
    // otherwise look it up (existOnly) or intern-and-look-up (!existOnly) via the global
    // dsSTRID_POOL singleton (GetStrIdPool()).
    dsSTRID(const char *str, bool existOnly);
    // 0x8252B938 — same as above, but the source characters come from a dsTSTRING<char>
    // (measures `s.pBuffer->strLen` instead of scanning for a NUL).
    dsSTRID(const dsTSTRING<char> &s, bool existOnly);
    // 0x825280F0 — copy `other`'s already-interned id verbatim (no re-interning). The trailing
    // bool has no observed effect on the body; kept as an overload disambiguator against the
    // implicit compiler-generated copy constructor.
    dsSTRID(const dsSTRID &other, bool);
    // 0x825280E0 — default: adopt the process-wide interned id of the empty string.
    dsSTRID();

    // 0x82528108
    const char *CStr() const;

    // 0x82528118 — character-wise (strcmp-style) equality against a NUL-terminated literal.
    // DEVIATION from an earlier guess in this header: the body does NOT intern `rhs` or compare
    // pooled pointers — it walks both strings byte-by-byte until a mismatch or a NUL in `id`.
    bool operator==(const char *rhs) const;

    // dsSTRID::operator!=(const dsSTRID&) — inequality against another interned id. boundary
    // (external to this batch; referenced by HALO_SOUND_LIST::ParseLst).
    bool operator!=(const dsSTRID &rhs) const; // boundary

    // Added by the HALO_SOUND_PARAMS re-source pass (used by HALO_SOUND_PARAMS::Init).
    bool IsValid() const; // ?IsValid@dsSTRID@@QBA_NXZ -- true when `id` is not the pool's empty-string id
    bool IsEmpty() const; // ?IsEmpty@dsSTRID@@QBA_NXZ -- true when `id` IS the pool's empty-string id
};
