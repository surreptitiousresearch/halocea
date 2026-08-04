#pragma once
#include "propBASE.h"
#include "../ds/dsVECTOR.h"
// ws-engine prop subsystem: the property/component aggregate embedded (by value) in every
// entity that hosts prop-family objects (iaIACTOR::propContainer). Extends propBASE (so a
// propCONTAINER is itself usable wherever a propBASE is expected) with two owned lists: the
// entity's own properties and its components.
// DB-verified layout (types_members propCONTAINER): <propBASE base>@0 (40),
// properties@40 (dsVECTOR<propBASE*,8>, 20), components@60 (dsVECTOR<propBASE*,8>, 20) — size 80.

struct propCONTAINER : propBASE {
    dsVECTOR<propBASE *, 8> properties; // 0x28 this entity's own properties
    dsVECTOR<propBASE *, 8> components; // 0x3C attached component objects

    // Container-wide visibility broadcast, invoked by entENTITY show/hide. Hide/Show forward the
    // whole-entity transition; ShowToCamera/HideFromCamera forward a per-camera transition.
    // boundary.
    void Hide();
    void Show();
    void ShowToCamera(int idCamera);
    void HideFromCamera(int idCamera);
};
