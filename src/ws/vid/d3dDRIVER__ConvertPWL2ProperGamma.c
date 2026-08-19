/* ?ConvertPWL2ProperGamma@d3dDRIVER@@QBAMM@Z @0x82696518 */
#include "headers/d3d_render_state_boundary.h"

// 0x82696538
/* DEVIATION: param respelled double->float per the mangling (@@QBAMM@Z = float(float)); body
   0x82696518-5C contains zero float-conversion insns, f1 flows untouched into the float(float)
   callees. Identity cast stripped per the FP-identity-cast precedent. 2026-08-18 */
float d3dDRIVER::ConvertPWL2ProperGamma(float C) const
{
    if (dbg_correctPWLGamma.value && this->enablePWLGamma)
        return d3dSRGBGamma(d3dPWLDeGamma(C));

    return C;
}
