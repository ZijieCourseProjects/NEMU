#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */
	ph=(Elf32_Phdr *)((uint8_t*)elf + elf->e_phoff);
	Elf32_Phdr *eph=ph+elf->e_phnum;
	for(; ph<eph;ph++ ) {
		if(ph->p_type == PT_LOAD) {
		  ph->p_paddr=mm_malloc(ph->p_vaddr,ph->p_memsz);
			ramdisk_read((void*)(ph->p_paddr),ph->p_offset,ph->p_filesz);

#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }
#endif
			if (ph->p_memsz>ph->p_filesz)
				memset((void*)(ph->p_paddr+ph->p_filesz),0,ph->p_memsz-ph->p_filesz);
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

	create_video_mapping();

	write_cr3(get_ucr3());
#endif

	return entry;
}
