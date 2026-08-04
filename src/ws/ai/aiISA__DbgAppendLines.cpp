#include "../../headers/ws/ai/aiISA.h"

// aiISA::DbgAppendLines(dsTSTRING<char>&) @ 0x832A14B0
// ?DbgAppendLines@aiISA@@UAAXAAV?$dsTSTRING@D@@@Z (virtual)
// Append this module's debug status ("ISA: Enabled." + alerted flag) to `line`.
void aiISA::DbgAppendLines(dsTSTRING<char> &line)
{
    line.Insert(line.pBuffer->strLen, "ISA: Enabled.", -1);

    const char *alertedText = this->IsAlerted() ? " Alerted = true." : " Alerted = false.";
    line.Insert(line.pBuffer->strLen, alertedText, -1);
}
