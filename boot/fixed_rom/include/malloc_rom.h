
extern void rom_boot_malloc_init(td_u32 heap_start_addr, td_u32 heap_end_addr);
extern void *rom_boot_malloc(unsigned int size);
extern int rom_boot_free(void *addr);