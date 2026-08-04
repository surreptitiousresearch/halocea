#include "m3d_boundary.h"

// m3dClamp(float,float,float,float,float) @ 0x823CD030
// DEVIATION: mangle is ?m3dClamp@@YAMMMMMM@Z (return M + 5x M) and the disasm prologue
// (`__savefpr_27`, f1..f5 -> f31..f27) confirms exactly 5 float parameters, not 6 as an
// initial guess from the mangle token count suggested.
//
// Clamps `value` into the (order-independent) range [rangeLo, rangeHi], then linearly
// remaps the clamped value from that range onto [outAtLo, outAtHi] (outAtLo/outAtHi are
// paired with whichever of rangeLo/rangeHi is numerically smaller/larger, so the mapping
// direction is preserved even if rangeLo > rangeHi). Guards the remap denominator against
// a degenerate (near-zero-width) range with the shipped 1e-6 epsilon.
float m3dClamp(float outAtLo, float outAtHi, float rangeLo, float rangeHi, float value)
{
    _m3dCheckValid(outAtLo);
    _m3dCheckValid(outAtHi);
    _m3dCheckValid(rangeLo);
    _m3dCheckValid(rangeHi);
    _m3dCheckValid(value);

    bool loIsRangeLo = (rangeLo - rangeHi) >= 0.0f; // true when rangeLo is actually the larger bound
    float rangeMin = loIsRangeLo ? rangeHi : rangeLo;
    float rangeMax = loIsRangeLo ? rangeLo : rangeHi;
    float outAtMin = loIsRangeLo ? outAtHi : outAtLo;
    float outAtMax = loIsRangeLo ? outAtLo : outAtHi;

    float clampedLo = (value - rangeMin) >= 0.0f ? value : rangeMin;      // max(value, rangeMin)
    float clampedValue = (clampedLo - rangeMax) >= 0.0f ? rangeMax : clampedLo; // min(clampedLo, rangeMax)

    float rangeWidth = rangeMax - rangeMin;
    bool degenerate = (rangeWidth - 0.000001f) < 0.0f;
    float safeWidth = degenerate ? 1.0f : rangeWidth;   // dodge div-by-zero on a degenerate range
    float remapFrom = degenerate ? rangeMin : clampedValue; // forces numerator to exactly 0 in the degenerate case

    return (remapFrom - rangeMin) * ((outAtMax - outAtMin) / safeWidth) + outAtMin;
}
