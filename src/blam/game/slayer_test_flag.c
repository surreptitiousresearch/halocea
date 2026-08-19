/* slayer_test_flag @0x838157A8 */
#include <stdint.h>
#include "headers/slayer_globals.h"

uint8_t slayer_test_flag(int flag)
{
    return flag == 1 ? 1 : 0;
}
