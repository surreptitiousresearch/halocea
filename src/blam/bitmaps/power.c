/* power @0x836D8460 */
#include <math.h>

/* DEVIATION: decompiler shows double params; prototype is float; PPC FPRs hold both */
float power(float a, float b)
{
    return (float)pow((double)a, (double)b);
}
