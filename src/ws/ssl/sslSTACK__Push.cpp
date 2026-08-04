#include "sslSTACK.h"
#include "sslSTACK_DATA.h"

// 0x82AF4508 — sslSTACK::Push. Append a fresh operand initialised to (TYPE_INT, 0) and return a
// reference to the newly-created top slot.
sslSTACK_DATA &sslSTACK::Push()
{
    sslSTACK_DATA newEntry;
    newEntry.type = sslSTACK_DATA::TYPE_INT;
    newEntry.int_val = 0;
    mainStack.PushBack(newEntry);
    newEntry.Cleanup(); // release the temporary's payload; the vector copy owns its own
    return mainStack.Back();
}
