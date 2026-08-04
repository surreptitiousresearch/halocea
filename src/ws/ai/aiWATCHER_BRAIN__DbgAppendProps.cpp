#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"        // aiWATCHER base — fsmPropFlags
#include "../../headers/ws/ds/dsTSTRING.h"

// aiWATCHER_BRAIN::DbgAppendProps @ 0x8328F4C0
// ?DbgAppendProps@aiWATCHER_BRAIN@@QBAXAAV?$dsTSTRING@D@@@Z
//
// aiDEBUG overlay helper: append a tab-separated dump of the aiWATCHER-base FSM property flag set to
// `line`, four flags per row. Each set bit prints its name; each clear bit prints a "." placeholder.
// (The base's fsmPropFlags lives at aiWATCHER offset 4; the aiWATCHER_BRAIN header models the base
//  as an opaque blob, so it is reached through an aiWATCHER view of `this`.)
void aiWATCHER_BRAIN::DbgAppendProps(dsTSTRING<char> &line) const
{
    const int flags = reinterpret_cast<const aiWATCHER *>(this)->fsmPropFlags.val;

    // Row 1
    line.Insert(line.pBuffer->strLen, (flags & (1 << 0))  ? "MINIZOOM\t"   : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 1))  ? "REACHED_MZ\t" : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 2))  ? "SHOOTING\t"   : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, "\r\n", -1);
    // Row 2
    line.Insert(line.pBuffer->strLen, (flags & (1 << 4))  ? "MZ_FLOOR_HO\t"  : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 5))  ? "MZ_WLN_HO\t"    : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 6))  ? "UNCONTROLLED\t" : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 7))  ? "FALLING\t\t"    : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, "\r\n", -1);
    // Row 3
    line.Insert(line.pBuffer->strLen, (flags & (1 << 8))  ? "HIDEOUT\t\t"    : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 9))  ? "REACHED_COVER\t": ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 10)) ? "LEAVE_HO\t"     : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 11)) ? "SMART_OBJ\t"    : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, "\r\n", -1);
    // Row 4
    line.Insert(line.pBuffer->strLen, (flags & (1 << 12)) ? "LOW\t\t"        : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 13)) ? "LEFT\t\t"       : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 14)) ? "RIGHT\t\t"      : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 15)) ? "POS_SIDE\t"     : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, "\r\n", -1);
    // Row 5
    line.Insert(line.pBuffer->strLen, (flags & (1 << 16)) ? "HI_G_HIT\t"     : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 17)) ? "LOW_G_HIT\t"    : ".\t\t", -1);
    line.Insert(line.pBuffer->strLen, (flags & (1 << 18)) ? "WEIGHTLESS\t"   : ".\t\t", -1);
    const int posBit19 = line.pBuffer->strLen;
    line.Insert(posBit19, (flags & (1 << 19)) ? "WLN_JUMP_FLY\t" : ".\t\t", -1);
}
