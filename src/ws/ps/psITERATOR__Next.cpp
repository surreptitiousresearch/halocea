/* ?Next@psITERATOR@@QAAXXZ @0x82516A18 */
#include "psITERATOR.h"
// 0x82516A18 — advance the cursor by one, clamped at the end of the collected record set.

void psITERATOR::Next()
{
    int pos = this->pos;
    if (pos < this->records.nElem)
        this->pos = pos + 1;
}
