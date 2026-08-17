/* ?IsDone@psITERATOR@@QBAHXZ @0x825169F8 */
#include "psITERATOR.h"
// 0x825169F8 — iteration finished when the cursor has reached the end of the collected record set.
// DEVIATION: the decompiler renders this as the expanded signed-compare idiom
// (pos>>31) + (pos >= (unsigned)nElem) + ((unsigned)nElem>>31); that is the compiler's lowering of
// the signed `pos >= records.nElem`, restored here.

int psITERATOR::IsDone() const
{
    return this->pos >= this->records.nElem;
}
