/* ai_dispose @ 0x836E7680 — tear down AI subsystems (reverse of ai_initialize) */
extern void ai_communication_dispose(void);
extern void ai_script_dispose(void);
extern void encounters_dispose(void);
extern void props_dispose(void);
extern void actors_dispose(void);
extern void paths_dispose(void);
void ai_dispose(void)
{
    ai_communication_dispose();
    ai_script_dispose();
    encounters_dispose(); /* attested void */
    props_dispose();
    actors_dispose(); /* attested void */
    paths_dispose();
}
