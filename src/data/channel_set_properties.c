/* channel_set_properties @ 0x8445D3E0 (.data, 8 bytes)
 * DB applied_types: void (__fastcall *channel_set_properties)(__int16, platform_sound_channel_properties *, unsigned __int8, __int16);
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 * data-bss function pointer (4 bytes). DB decl __fastcall
 * dropped (nonportable); ledger gap 8 overcounts.
 */
#include "../headers/platform_sound_channel_properties.h" /* narrow: HALO_CHANNEL.h pulls ws/snd C++ chain */

void (*channel_set_properties)(short, platform_sound_channel_properties *, unsigned char, short);
