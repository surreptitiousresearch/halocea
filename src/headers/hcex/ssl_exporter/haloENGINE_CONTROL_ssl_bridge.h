#pragma once
#include "../../ws/ds/dsTSTRING.h"
#include "../../ws/ds/REF_TYPE.h"
#include "../../ws/ds/dsREF_VECTOR.h"

/* RECONCILED (haloENGINE_CONTROL owning pass, engine_control batch): this file used to carry its own
 * 13-method forward declaration of haloENGINE_CONTROL because the canonical header
 * (src/headers/hcex/haloENGINE_CONTROL.h) was, at the time, a C-only boundary view incompatible with
 * the C++ template `dsTSTRING<char>` these SSL_EXPORTER bindings need. The canonical header is now a
 * dual C/C++ view (real class body under `#ifdef __cplusplus`, covering all 32 reversed
 * haloENGINE_CONTROL + nested EVENT_DISP_haloENGINE_CONTROL methods) and is safe for this C++
 * translation unit to include directly, so the duplicate local forward declaration is dropped here
 * per the note that used to live in this file. */
#include "../haloENGINE_CONTROL.h"
