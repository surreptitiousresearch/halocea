/* hcex_collect_crc @0x823BFA48 — record this tick's game-state CRC into the rolling crc_list ring
 * buffer (capacity 64: the oldest entry is dropped once full), inserted at the front (index 0) so
 * crc_list[n] holds the CRC from n ticks ago. Paired with hcex_check_crc, which walks the local
 * co-op message handler's last raw input tick backwards into this same history to spot a CRC
 * mismatch (desync) between the local and remote simulation. */

#include <stdint.h>
#include "../headers/ws/ds/dsCONST_ARRAY.h"

extern dsCONST_ARRAY<int, 64> crc_list;
extern "C" uint32_t game_state_calculate_crc(void);

void hcex_collect_crc(void)
{
    if (crc_list.nEntry == 64)
        crc_list.Remove(63);

    int crc = game_state_calculate_crc();
    crc_list.Insert(0, &crc, 1);
}
