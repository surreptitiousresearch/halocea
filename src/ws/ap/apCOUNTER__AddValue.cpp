// apCOUNTER::AddValue @0x82715C34 — accumulate a sample into counterUnits[index]: add `value` to
// the running `value`, bump numValueChanged, recompute the running average
// ((old average * (n-1)) + new value) / n, and raise valuePeak when the new value exceeds it.
#include "../../headers/ws/ap/apCOUNTER.h"

void apCOUNTER::AddValue(int index, float value) // DB proto ?AddValue@apCOUNTER@@QAAXHM@Z (int,float)
{
    apCOUNTER_UNIT &unit = counterUnits[index];

    float newValue = unit.value + (float)value;
    int   n         = unit.numValueChanged + 1;

    unit.value           = newValue;
    unit.numValueChanged = n;
    unit.valueAverage    = ((float)(n - 1) * unit.valueAverage + newValue) / (float)n;

    if (newValue > unit.valuePeak)
        unit.valuePeak = newValue;
}
