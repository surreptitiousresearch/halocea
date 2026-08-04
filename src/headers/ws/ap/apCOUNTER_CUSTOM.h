#pragma once
#include "apCOUNTER.h"

// ws-engine ap subsystem — a self-registering custom performance counter. DB-verified: derives
// apCOUNTER with no additional data members. The constructor registers the counter under a
// slash-delimited path with a set of display-unit labels.
struct apCOUNTER_CUSTOM : apCOUNTER {
    apCOUNTER_CUSTOM(const char *path, const char *unitLabels, const char *group);
};
