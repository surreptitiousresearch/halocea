#pragma once
// ws-engine ds: result code returned by a ds::Tokenize<T> callback object's
// ProcessToken/ProcessDelim methods, and by ds::Tokenize itself.
// DB-verified (types_enum_values ds::TOKENIZE_RESULT): CONTINUE=0, BREAK=1, ERROR=2.

namespace ds {

typedef enum TOKENIZE_RESULT
{
    TOKENIZE_CONTINUE = 0,
    TOKENIZE_BREAK    = 1,
    TOKENIZE_ERROR    = 2,
} TOKENIZE_RESULT;

// 0x82C65D5C-ish (per-instantiation, e.g. ds::Tokenize<TEX_USAGE_PROCESSOR>) -- splits `src` on
// delimiters found in `delims`, feeding each token/delimiter to `processor`'s ProcessToken /
// ProcessDelim callbacks until a token callback returns non-CONTINUE or the string is exhausted.
// Body not decompiled here (generic per-instantiation template driver). boundary.
template<class T>
TOKENIZE_RESULT Tokenize(const void *src, const void *delims, T &processor);

} // namespace ds
