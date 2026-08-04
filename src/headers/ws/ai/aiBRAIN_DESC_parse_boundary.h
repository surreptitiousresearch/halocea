#pragma once
#include "aiBRAIN_DESC.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
#include "../ps/psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// External symbols referenced by the aiBRAIN_DESC parser methods (ReadVoices/ReadEnemy/
// ReadSpeed(List)/ReadSubsystems) but not themselves reversed in this batch. boundary.

// Interned subsystem-type name ids used to validate the "subsystems" section values.
extern dsSTRID ssTYPE_NULL;
extern dsSTRID ssTYPE_IDLE;
extern dsSTRID ssTYPE_BATTLE;
extern dsSTRID ssTYPE_FULL;

// 0x83187740 — read `key` from `psRead` as a subsystem-type name, validate it against
// `acceptableValues`, and return the resolved type (or `def` when missing/invalid).
aiBRAIN_DESC::aiSUBSYS_TYPE ReadAndCheckSystemType(psSECTION_INTERFACE *psRead, const char *key,
                                                   aiBRAIN_DESC::aiSUBSYS_TYPE def,
                                                   dsVECTOR<dsSTRID, 8> *acceptableValues);

// Coerce a boxed dsDATA holding a psSECTION into `out`; returns whether the coercion succeeded.
int psDataToSection(const dsDATA *data, psSECTION *out);

// Engine diagnostic log (printf-style).  boundary.
extern void _apLog(const char *fmt, ...);
