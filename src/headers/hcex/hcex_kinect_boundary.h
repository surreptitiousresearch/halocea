#pragma once
/* Boundary declarations for the ws-engine Kinect input subsystem (gsINP_KINECT) touched by the
 * HCEX bridge's hcex_change_kinect_state. Only the load/unload `state` bitfield is actually touched
 * by reversed HCEX code.
 *
 * DEVIATION (2026-08-07, odr_dup drain): this header used to restate the whole 844-byte
 * gsINP_KINECT layout as a flat plain-C struct whose helper value-types were kept opaque, beside
 * the canonical C++ home at src/headers/ws/inp/gsINP_KINECT.h — one `error: redefinition` in
 * header_layout's probe TU, and two spellings of one type whose ordering decided what a TU saw.
 * The canonical body wins on DB evidence: types_members gsINP_KINECT row 0 is an ANONYMOUS
 * BASE-CLASS member of type msgADDR at offset 0 (is_baseclass=1, size 32), which
 * `struct gsINP_KINECT : public msgADDR` models and the flat `struct msgADDR base;` member did
 * not; the canonical header also carries the real DB layouts of inpKINECT_SKEL (332),
 * inpKINECT_ARM_HANDLES (28), gsINP_KINECT_ARM (16), gsINP_KINECT_UI_FRAME (20) and
 * EVENT_DISP_gsINP_KINECT (8) where this copy carried `unsigned char _opaque[N]` stand-ins.
 * Field offsets were identical in both, so no consumer changed: hcex_change_kinect_state.cpp
 * reads `->state.state` only, and `state` is apSTATE_T<unsigned long> at 0x344 either way. */

#include "../ws/inp/gsINP_KINECT.h"

extern gsINP_KINECT *gsInpKinect;

/* 0x826EE930 / 0x826EEA50 — boundary, external to this batch. */
extern void gsINP_KINECT_Load(gsINP_KINECT *self);
extern void gsINP_KINECT_Unload(gsINP_KINECT *self);
