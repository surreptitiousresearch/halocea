#pragma once
/* virtual-keyboard field validation modes — selects which characters virtual_keyboard_is_input_valid
 * accepts. Values from the database anonymous enum _50D9824DA78FEC0B38C59598451CAE0E
 * (same enum block that carries VIRTUAL_KEYBOARD_TAG / caption / language constants). */

enum virtual_keyboard_validate_mode
{
    _virtual_keyboard_validate_none              = 0,
    _virtual_keyboard_validate_profile_name      = 1,
    _virtual_keyboard_validate_playlist_profile_name = 2,
    _virtual_keyboard_validate_not_empty         = 3,
    _virtual_keyboard_validate_internet_address  = 4,
    _virtual_keyboard_validate_number            = 5,
};
