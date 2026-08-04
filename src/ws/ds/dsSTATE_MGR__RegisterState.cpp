#include "dsSTATE_MGR.h"

// dsSTATE_MGR::RegisterState @ 0x824F7058
// Intern the C-string name into a dsSTRID, then forward to the id-based overload.
int dsSTATE_MGR::RegisterState(const char *id, bool isToggleEventOnChange)
{
    dsSTRID internedId(id, 0);
    return this->RegisterState(internedId, isToggleEventOnChange);
}
