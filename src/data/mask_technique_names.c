/* mask_technique_names @ 0x84181C18 (.data, 3072 bytes)
 * DB applied_types: char mask_technique_names[24][128];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 4D 61 73 6B 44 65 74 61 69 6C 42 65 66 6F 72 65
 *   +0x0010: 52 65 66 6C 65 63 74 69 6F 6E 42 69 61 73 65 64
 *   +0x0020: 4D 75 6C 74 69 70 6C 79 00 00 00 00 00 00 00 00
 *   +0x0030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0050: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0060: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0070: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0080: 4D 61 73 6B 44 65 74 61 69 6C 42 65 66 6F 72 65
 *   +0x0090: 52 65 66 6C 65 63 74 69 6F 6E 4D 75 6C 74 69 70
 *   +0x00A0: 6C 79 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00B0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00C0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00D0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00E0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00F0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0100: 4D 61 73 6B 44 65 74 61 69 6C 42 65 66 6F 72 65
 *   +0x0110: 52 65 66 6C 65 63 74 69 6F 6E 42 69 61 73 65 64
 *   +0x0120: 41 64 64 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0180: 4D 61 73 6B 44 65 74 61 69 6C 41 66 74 65 72 52
 *   +0x0190: 65 66 6C 65 63 74 69 6F 6E 42 69 61 73 65 64 4D
 *   +0x01A0: 75 6C 74 69 70 6C 79 00 00 00 00 00 00 00 00 00
 *   +0x01B0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x01C0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x01D0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x01E0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x01F0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0200: 4D 61 73 6B 44 65 74 61 69 6C 41 66 74 65 72 52
 *   +0x0210: 65 66 6C 65 63 74 69 6F 6E 4D 75 6C 74 69 70 6C
 *   +0x0220: 79 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0230: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0240: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0250: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0260: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0270: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   ... 2432 further bytes elided; full hex in .sweep/data_image.tsv
 * Blam .lib data-global storage definition (init batch 7).
 */
/* mask_technique_names @0x84181C18, DB extent 3072 = char[24][128] technique-name table. */
char mask_technique_names[24][128] = {
    "MaskDetailBeforeReflectionBiasedMultiply",
    "MaskDetailBeforeReflectionMultiply",
    "MaskDetailBeforeReflectionBiasedAdd",
    "MaskDetailAfterReflectionBiasedMultiply",
    "MaskDetailAfterReflectionMultiply",
    "MaskDetailAfterReflectionBiasedAdd",
    "MaskInverseDetailBeforeReflectionBiasedMultiply",
    "MaskInverseDetailBeforeReflectionMultiply",
    "MaskInverseDetailBeforeReflectionBiasedAdd",
    "MaskInverseDetailAfterReflectionBiasedMultiply",
    "MaskInverseDetailAfterReflectionMultiply",
    "MaskInverseDetailAfterReflectionBiasedAdd",
    "MaskDetailBeforeReflectionBiasedMultiplyComplexFog",
    "MaskDetailBeforeReflectionMultiplyComplexFog",
    "MaskDetailBeforeReflectionBiasedAddComplexFog",
    "MaskDetailAfterReflectionBiasedMultiplyComplexFog",
    "MaskDetailAfterReflectionMultiplyComplexFog",
    "MaskDetailAfterReflectionBiasedAddComplexFog",
    "MaskInverseDetailBeforeReflectionBiasedMultiplyComplexFog",
    "MaskInverseDetailBeforeReflectionMultiplyComplexFog",
    "MaskInverseDetailBeforeReflectionBiasedAddComplexFog",
    "MaskInverseDetailAfterReflectionBiasedMultiplyComplexFog",
    "MaskInverseDetailAfterReflectionMultiplyComplexFog",
    "MaskInverseDetailAfterReflectionBiasedAddComplexFog",
};
