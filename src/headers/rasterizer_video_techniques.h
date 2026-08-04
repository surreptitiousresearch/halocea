#pragma once

#include "rasterizer_shader_helpers.h"

/* Screen-effect ("video on/off") technique handles. */
extern unsigned int hVideoOn;
extern unsigned int hVideoOffNonConvolved;
extern unsigned int hVideoOffConvolvedMask;
extern unsigned int hVideoOffConvolveMaskThreeStage;
extern unsigned int hVideoOffConvolvedMaskFilterLightAndDesaturation;
extern unsigned int hVideoOffConvolvedMaskFilterLight;
extern unsigned int hVideoOffConvolvedMaskFilterDesaturation;
extern unsigned int hVideoOffConvolved;
extern unsigned int hVideoOffConvolvedFilterLightAndDesaturation;
extern unsigned int hVideoOffConvolvedFilterLight;
extern unsigned int hVideoOffConvolvedFilterDesaturation;
