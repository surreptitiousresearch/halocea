#include "psSECTION.h"

/* HCEX boundary flat-bridge for psSECTION::operator= @ 0x8251A1A0 (re-sourced in
 * psSECTION__operatorAssign.cpp). hcex_init_breakable_surfaces.cpp copies a section handle
 * through this flattened spelling; the shim only adapts pointer-vs-reference surface. */
void psSECTION__assign(psSECTION *dst, const psSECTION *src)
{
    *dst = *src;
}
