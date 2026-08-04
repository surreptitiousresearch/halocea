#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/ai_setup_subsystems_boundary.h"

// aiBRAIN::SetupBody @ 0x8317F838
// ?SetupBody@aiBRAIN@@MAAXXZ  (protected virtual)
//
// Build the brain's body interface and its owned sub-modules (die / weapon / hideout system) from
// the descriptor-driven subsystem-type selectors. Each module is only installed when it is still
// the shared "null" singleton, so re-entry is idempotent.
void aiBRAIN::SetupBody()
{
    // Descriptor object (iaIACTOR::spDesc.pointee at this+0x54). Indexed as packed selector cells;
    // also viewed through the typed brain descriptor for the named subsystem-type selectors.
    const unsigned char *desc = *reinterpret_cast<const unsigned char *const *>(
        reinterpret_cast<const char *>(this) + 0x54);
    const aiBRAIN_DESC *pDesc = reinterpret_cast<const aiBRAIN_DESC *>(desc);

    // Body interface: NULL(1) or FULL(4) — selector word ssTypeBody.
    if (!this->body.pointee)
    {
        int bodyType = pDesc->ssTypeBody;
        if (bodyType == 1)
            this->body.Reset<aiBODY_ZERO>(new aiBODY_ZERO(this));
        else if (bodyType == 4)
            this->body.Reset<aiBODY>(new aiBODY(this));
        else
            _apLog("~AI,Error~Wrong body subsystem type (not NULL / FULL).");
    }

    aiBODY_IFACE *body = this->body.pointee;

    // Death module: install a FULL aiDIE when the body still holds the shared null-die singleton.
    if (mdlGetSysSingleton<aiDIE_BASE>()->pointee == body->die.pointee)
        body->die.Reset<aiDIE>(new aiDIE(this));

    // Weapon module: likewise for the weapon subsystem.
    if (mdlGetSysSingleton<aiWEAPON_BASE>()->pointee == body->wpn.pointee)
        body->wpn.Reset<aiWEAPON>(new aiWEAPON(this));

    // Hideout system: install when absent. A FULL aiHO_SYS is created when any of three descriptor
    // hideout-enable flags is set (bytes 0x1E8, 0x201, 0x202), otherwise a bare null aiHO_SYS_BASE.
    if (!body->hoSys.pointee)
    {
        if (desc[0x1E8] || desc[0x201] || desc[0x202])
            body->hoSys.Reset<aiHO_SYS>(new aiHO_SYS(this));
        else
            body->hoSys.Reset<aiHO_SYS_BASE_INST>(new aiHO_SYS_BASE_INST()); // null hideout system
    }
}
