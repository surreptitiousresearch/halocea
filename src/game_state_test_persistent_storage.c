/* game_state_test_persistent_storage @0x83684B38 — probe the saved-game header in persistent storage
 * (device/content id 4489216, 332-byte header). On success returns the saved map name and difficulty
 * and reports whether the stored data is corrupted; on failure returns difficulty=normal(1) and an empty
 * map name. Within the header buffer the map-name string lives at +0x04 and the difficulty word at
 * +0x126. */

#include <stdint.h>

extern uint8_t game_state_read_header_from_persistent_storage(void *header, unsigned int *header_checksum, int header_size, int buffer_size, uint8_t *corrupted);
extern char *strcpy(char *dest, const char *src);

uint8_t game_state_test_persistent_storage(char *map_name, int16_t *difficulty, uint8_t *corrupted)
{
    unsigned char header[332];  /* read in whole; fields accessed at fixed offsets below */
    unsigned int  content_id[4];

    const char    *header_map_name   = (const char *)&header[0x04];
    const int16_t *header_difficulty = (const int16_t *)&header[0x126];

    if (game_state_read_header_from_persistent_storage(header, content_id, 332, 4489216, corrupted))
    {
        *difficulty = *header_difficulty;
        strcpy(map_name, header_map_name);
        return 1;
    }
    else
    {
        *difficulty = 1;
        *map_name = 0;
        return 0;
    }
}
