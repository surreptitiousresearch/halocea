/* hcex_aim_assist_coeff @ 0x84175FE8 (.data, 4 bytes)
 * DB applied_types: float hcex_aim_assist_coeff;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x3F800000
 * data-init scalar, float.
 * P4 sweep flagged addi at +0x4: reaches a SEPARATE global (DB unk_84175FEC) that is anchored on a
 * DIFFERENT base (0x84175F30, not hcex_aim_assist_coeff), used as the base of another structure.
 * Not a second coefficient of this scalar. Type float is correct.
 */
float hcex_aim_assist_coeff = 1.0f;
