#include <stdint.h>
/* prepare_buffer_to_send @0x83810638 — writes a packed 2-byte network header into `buffer`: bits [15:4] hold
 * (content_size + 2), bits [3:2] hold (type & 3). Returns `buffer` unchanged (null-safe: no-op on null). */

uint16_t * prepare_buffer_to_send(uint8_t type, void *buffer, uint16_t content_size)
{
    if (buffer)
        *(uint16_t *)buffer = (uint16_t)(16 * (content_size + 2)) | ((4 * type) & 0xC);
    return (uint16_t *)buffer;
}
