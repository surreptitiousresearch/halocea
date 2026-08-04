#include <math.h>

/* DEVIATION: decompiler shows double param; prototype is float; PPC FPRs hold both */
float arccosine(float x)
{
    return (float)acos((double)x);
}
