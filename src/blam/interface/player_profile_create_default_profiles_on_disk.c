/* player_profile_create_default_profiles_on_disk @ 0x837654E8 — build the two default
 * .sav profile images. The decompiler builds each profile image directly in a raw word buffer
 * (a stack player_profile), so the field writes are reproduced as raw indices with the
 * equivalent named-field meaning noted in comments.
 *
 * CAVEAT — faithfully reconstructed; despite its name the function never writes anything to disk.
 * It builds the full 1,980-byte image, formats "%s\%02d.sav" and creates a file_reference — and
 * then the loop simply repeats and the function returns (bl file_reference_create_from_path_absolute
 * @0x83765608 is the LAST call; 0x8376560C-0x8376561C are the loop increment and
 * `b __restgprlr_16`). There is no file-create/write/close call anywhere in the 78 instructions.
 * The stack image and the file_reference are both discarded. Do not add the missing write. */

#include <stdint.h>
#include <string.h>
#include "headers/player_profile.h"
#include "headers/file_reference.h"
#include "headers/saved_game_files_path_type.h"

#include "headers/file_reference.h"
extern int cache_file_is_open(void);
extern char *saved_game_files_get_path(int16_t path_type);
extern int _snprintf_0(char *, unsigned int, const char *, ...);
extern file_reference *file_reference_create_from_path_absolute(file_reference *reference, const char *path, uint8_t directory);

void player_profile_create_default_profiles_on_disk(void)
{
    int i;

    for ( i = 0; i < 2; ++i )
    {
        unsigned short img[990];   /* raw player_profile image (1980 bytes) */
        char path_buf[256];
        file_reference ref;
        const char *path;

        memset(img, 0, sizeof(img));
        ((unsigned char *)img)[1]   = 9;     /* profile_version region — typerec residue: raw on-disk serialization image, kept byte-faithful */
        img[141] = (unsigned short)-1;          /* primary_color_index */
        /* RAW (irreducible): controller_settings region (player_profile offset 0x12A =
         * &img[149]) is written as big-endian word-packed stores that straddle
         * adjacent unsigned __int8 members (e.g. img[150]=0x0300 sets
         * look_sensitivity=3 + invert_look=0 in one 16-bit store). Converting to
         * named controller_settings.<field> byte writes would change the store
         * width, so the raw serialization image is kept as the faithful form. */
        img[150] = 768;                         /* 0x12C look_sensitivity(=3) | 0x12D invert_look(=0) */
        img[151] = 0;                           /* 0x12E vibration_disabled | 0x12F flight_stick_aircraft_controls */
        *((unsigned char *)&img[152]) = 0;      /* 0x130 autocenter */
        img[142] |= ((unsigned short)i << 8) | 1; /* flags */
        memset(&img[153], 0, 0x15);             /* 0x132 game_control_to_xbox_buttons[21] */
        img[153] = 4;
        img[154] = 515;
        img[155] = 261;
        img[156] = 1543;
        img[157] = 3085;
        img[158] = 3599;
        img[148] = 0;                           /* 0x128 last_single_player_map_played */
        if ( i == 1 )
            *((unsigned char *)&img[150]) = 1;  /* 0x12C look_sensitivity (BE first byte), not invert_look */
        img[149] = 0;                           /* 0x12A button_preset | 0x12B joystick_preset */

        cache_file_is_open();
        path = saved_game_files_get_path(_saved_game_files_path_type_default_player_profile_path);
        _snprintf_0(path_buf, 0xFF, "%s\\%02d.sav"  /* escaped: original literal had a raw backslash */, path, i);
        file_reference_create_from_path_absolute(&ref, path_buf, 0);
    }
}
