/* director_camera_mode_names @ 0x841764E0 (.data, 20 bytes)
 * DB applied_types: const char *director_camera_mode_names[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x821136FC -> "following"
 *   +0x0004 = 0x821136F0 -> "orbiting"
 *   +0x0008 = 0x821136E8 -> "flying"
 *   +0x000C = 0x821136E0 -> "editor"
 *   +0x0010 = 0x821136D0 -> "first person"
 * const char *[5] into .rdata string literals.
 */
const char *director_camera_mode_names[5] =
{
	"following",	/* 0x821136FC */
	"orbiting",	/* 0x821136F0 */
	"flying",	/* 0x821136E8 */
	"editor",	/* 0x821136E0 */
	"first person"	/* 0x821136D0 */
};
