/* exception_code_get_string @0x83762F50 — map a Win32/NT exception code to its symbolic name string, or
 * NULL if unrecognized.
 *
 * DEVIATION: the decompiler renders every comparison as arithmetic on negative decimal literals
 * (`exception_code == -1073741681`, `exception_code + 1073741680`, etc.) rather than the standard NTSTATUS
 * hex constants they actually are. Every branch was cross-checked against the well-known Windows NTSTATUS
 * values (e.g. `-1073741681` == `0xC000008F` == `STATUS_FLOAT_INEXACT_RESULT`) and rewritten as a single
 * equivalent `switch` on the raw code — same set of codes mapping to the same strings, NULL otherwise.
 * Case labels use the standard EXCEPTION_* constants (headers/nt_exception_codes.h, winnt.h values). */

#include "headers/nt_exception_codes.h"

char * exception_code_get_string(unsigned int exception_code)
{
    switch ( exception_code )
    {
        case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
        case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
        case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
        case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
        case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
        case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
        case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
        case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
        case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
        case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
        case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
        default: return 0;
    }
}
