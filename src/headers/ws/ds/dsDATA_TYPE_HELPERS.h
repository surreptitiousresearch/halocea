#pragma once

// Convenience aggregate for the static-policy helper templates in the ds_data type machinery.
// Each concrete dsDATA_TYPE_IMPL<T> virtual override forwards to the matching static in one of
// these four policies. Names are the DB-authoritative mangled class names (?$dsDATA_TYPE_STREAM /
// _CONSTRUCT / _PACK, and dsDATA_TYPE_CLASS as the reflection dispatch target); bodies are
// reversed per concrete T as explicit specializations under src/hcex/.
//
// This header used to carry its OWN copy of all four class bodies, which made each of them a
// file-scope redefinition against its canonical <type>.h -- `error: redefinition` in the
// header_layout probe TU, and a layout/signature hazard because which body a TU saw depended on
// include order. It is now purely the four canonical includes; every declaration it used to make
// lives in exactly one place:
//
//   dsDATA_TYPE_STREAM<T>     ws/ds/dsDATA_TYPE_STREAM.h     (identical body -- nothing lost)
//   dsDATA_TYPE_PACK<T>       ws/ds/dsDATA_TYPE_PACK.h       (identical body -- nothing lost)
//   dsDATA_TYPE_CLASS<T>      ws/ds/dsDATA_TYPE_CLASS.h      (superset: the canonical adds
//                                                             Get/SetProperty to the IsFunc /
//                                                             IsProperty / CallFunc declared here)
//   dsDATA_TYPE_CONSTRUCT<T>  ws/ds/dsDATA_TYPE_CONSTRUCT.h  (adjudicated -- this header spelled
//                             the value cell `void **` while the DB mangling says `PAK`
//                             (`unsigned long *`), as this header's own comment recorded; the
//                             canonical uses the 4-byte-cell spelling and the three
//                             haloPLAYER_ACTIONS_ENV bodies that used `void **` were re-spelled)
#include "dsBIT_STREAM.h"   // kept: this header supplied the complete ds::BIT_STREAM to its
                            // consumers (the canonical dsDATA_TYPE_PACK.h only forward-declares it)
#include "dsDATA_TYPE_STREAM.h"
#include "dsDATA_TYPE_CONSTRUCT.h"
#include "dsDATA_TYPE_PACK.h"
#include "dsDATA_TYPE_CLASS.h"
