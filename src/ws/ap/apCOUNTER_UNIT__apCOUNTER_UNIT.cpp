#include "apCOUNTER_UNIT.h"
#include "../ds/dsTSTRING.h"

// apCOUNTER_UNIT::apCOUNTER_UNIT @ 0x82715988
apCOUNTER_UNIT::apCOUNTER_UNIT(const char *name)
{
    this->name.pBuffer = nullptr;
    this->name.UnsafeInit(name, -1, 0);
    this->state.state = 1;
    this->color = 0;
    this->myProfiler = nullptr;
    this->graphScale = 1.0f;
    this->numValueChanged = 0;
    this->threshold = 0.0f;
    this->value = 0.0f;
    this->valueAverage = 0.0f;
    this->valueAverageOut = 0.0f;
    this->valuePeak = 0.0f;
    this->valuePeakOut = 0.0f;
}
