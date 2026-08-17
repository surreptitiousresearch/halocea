/* ?SetToDsStrId@sslSTACK_DATA@@QAAAAV1@VdsSTRID@@@Z @0x82AF0788 */
#include "sslSTACK_DATA.h"
#include "../ds/dsSTRID.h"

// 0x82AF0788 — sslSTACK_DATA::SetToDsStrId. dsSTRID is a 4-byte value (an interned const char*);
// its payload is stored in the scalar slot and tagged TYPE_DSSTRID.
sslSTACK_DATA &sslSTACK_DATA::SetToDsStrId(dsSTRID id)
{
    Cleanup();
    int_val = (int)id.id; // interned string pointer stored in the scalar arm
    type = TYPE_DSSTRID;
    return *this;
}
