#pragma once
// ws-engine ds: the const char* hashing policy used by dsLF_SORTED_MAP<const char*,...>.
// Empty type (no data members in the DB) — a stateless functor exposing the static string
// hash below.

typedef struct dsSTR_HASH {
    // 0x82527F58 — hash the NUL-terminated string `str` into a bucket index. `mul` and `add`
    // seed a per-4-byte-word (then per-tail-byte) mix: each chunk is (chunk*mul + add) shifted
    // left by a rolling 0..7 count and XORed into a 64-bit accumulator; the accumulator is then
    // reduced modulo 0x55555561 (1431655777).
    static int hash(const char *str, unsigned int mul, unsigned int add);
} dsSTR_HASH;
