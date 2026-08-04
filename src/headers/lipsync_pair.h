#pragma once
/* lipsync_pair — dsPAIR<int,mdlLIP_SYNCER*>: Blam object id -> lip-sync state. */

typedef struct mdlLIP_SYNCER mdlLIP_SYNCER;

#include "ws/ds/dsPAIR.h"
typedef dsPAIR<int, mdlLIP_SYNCER *> lipsync_pair; /* DB dsPAIR<int,mdlLIP_SYNCER *> */
