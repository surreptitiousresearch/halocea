#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN_DESC::ReadAll @ 0x83185538
// Parse every descriptor subsection from `psRead`, in the fixed order below. The first call is a
// virtual dispatch (vtable slot 0x2C) so a derived descriptor can extend the subsystem read; the
// rest are direct member calls. Only ReadSpeedList consumes the `hintErr` diagnostic context.
void aiBRAIN_DESC::ReadAll(const psSECTION &psRead, const dsTSTRING<char> &hintErr)
{
    this->ReadSubsystems();
    this->ReadAbilities(psRead);
    this->ReadPerception(psRead);
    this->ReadSpeedList(psRead, hintErr);
    this->ReadBehavModifiers(psRead);
    this->ReadVoices(psRead);
    this->ReadHealthDeath(psRead);
    this->ReadSFX(psRead);
    this->ReadEnemy(psRead);
    this->ReadNav(psRead);
    this->ReadAfraid(psRead);
    this->ReadSaveSections(psRead);
    this->ReadOther(psRead);
}
