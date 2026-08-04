#include <stdint.h>
#include "headers/ctf_globals.h"

uint8_t ctf_test_flag(int flag)
{
    return flag == 0 ? 1 : 0;
}
