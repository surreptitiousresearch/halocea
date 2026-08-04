#pragma once
// Boundary declarations for aiPLANNER::ProcessDebugKeys (a8_planner.cpp) — the input-system,
// console-callback and file-static globals it reaches that are not already declared elsewhere.
// Decl-only; bodies are the next frontier.

#include "../gs/gsINP_SYSTEM.h" // gsINP_SYSTEM::GetDbgContext / gsiCONTEXT::IsJustOn
#include "aiCON_CB.h"           // aiCON_CB::SendWatcheeToConsole

// The process input system (debug key context source). boundary.
extern gsINP_SYSTEM *gsSysInput;

// ?_aiConCb@@3PAVaiCON_CB@@A — the AI console callback (mirrors watchee state to the console). boundary.
extern aiCON_CB *_aiConCb;

// `anonymous namespace'::dbg_ai_PickCamPosForTrajCheck — one-shot request flag consumed by the
// trajectory-checker debug path. boundary.
extern bool dbg_ai_PickCamPosForTrajCheck;

// ?UpdateOffAI — apply the pending off-AI toggle at the end of the debug-key poll. boundary.
extern "C" void UpdateOffAI();
