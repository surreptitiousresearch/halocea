#pragma once
#include "dsTSTRING.h"
// ws-engine ds: static per-type error channel. Tracks the instance that last raised an
// error and the associated message string. Modeled with a single reversed slot (IsError);
// the two accessors it reads are static boundaries.

template<class T>
struct dsERROR_HANDLER {
    // 0x82AD9530 (dsERROR_HANDLER<dsDATA>) — true iff instance is the current error
    // instance AND a non-empty error message is set.
    static bool IsError(const T *instance);

    // 0x82ADA288 (dsERROR_HANDLER<dsDATA>) — if `instance` currently owns a non-empty error,
    // move that error string out (clearing the channel) and return it; otherwise return an
    // empty string. Returns dsTSTRING<char> by value (sret).
    static dsTSTRING<char> GetError(const T *instance);

    // Current error-owning instance / message; bodies external to this batch.
    static const T *GetCurrentInst();          // boundary
    // Reference to the process-wide error-string slot behind GetCurrentInst (mutated by GetError).
    static dsTSTRING<char> &GetErrorStr();      // boundary
    // Overwrite the current error-owning instance pointer (clears the channel when passed null).
    static void SetCurrentInst(const T *inst);  // boundary

    // 0x823C6B40 (dsERROR_HANDLER<haloENGINE_CONTROL>) — set `instance` as the current error owner
    // and overwrite the process-wide error-string slot with `error_msg`. Reversed per-instantiation
    // in src/hcex/dsERROR_HANDLER_<T>__SetError.cpp.
    static void SetError(const T *instance, const dsTSTRING<char> &error_msg);
};
