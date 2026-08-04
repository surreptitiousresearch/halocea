#pragma once

// ws-engine ai08 — base of a brain's "selene" mind module (aiMIND::selene pointee). Only the timer
// surface reached by aiMIND::UpdateTimers is declared; the full layout/bodies are the next frontier.
// boundary.

struct aiSELENE_BASE {
    void *__vftable; // 0x00 aiSELENE_BASE_vtbl*

    // ?UpdateTimers@aiSELENE_BASE@@UAAXM@Z — virtual: advance this module's timers by `dt` seconds.
    // Used by aiMIND::UpdateTimers.  boundary.
    void UpdateTimers(float dt);

    // ?IsZero@aiSELENE_BASE@@UBA_NXZ — virtual const: true for the null ("zero") selene arm. Used by
    // aiMIND::ZeroSelene / SetNormalSelene.  boundary.
    bool IsZero() const;
    // vtbl — per-frame selene (enemy-selection) update. Used by aiMIND::ProcessEnemySelection.
    void ProcessFrame();
};
