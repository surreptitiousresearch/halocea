#pragma once

// ws-engine ai08 — global debug toggles gating perception paths. Defined in the ai08 TU; declared
// here for the reversed consumers. boundary.

extern bool _isHandle; // when false, aiPERC::NoticeFlyObject early-outs (physics-object handling off)
extern bool _skip;     // when true, aiISA::Alert is suppressed
