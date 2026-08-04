#include <math.h>

/* DEVIATION: decompiler shows double params; prototype is float; PPC FPRs hold both */
float arctangent(float y, float x)
{
    return (float)atan2((double)y, (double)x);
}
