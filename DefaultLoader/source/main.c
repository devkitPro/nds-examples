
#include "nds.h"
#include "nds_file.h"

//---------------------------------------------------------------------------------
void LoadBin(const void * src, void * dst, int size) {
//---------------------------------------------------------------------------------
	uint32 * ram = (uint32 *)dst;
	uint32 * rom = (uint32 *)src;
	for (; size > 0; size -= 4) { *ram++ = *rom++; }
}

extern u8 __appended_data[];

//---------------------------------------------------------------------------------
int main(int argc, char ** argv) {
//---------------------------------------------------------------------------------

	NDS_header *NDS = (NDS_header *)__appended_data;

	LoadBin((void*)(NDS->arm9_rom_offset + NDS), (void*)NDS->arm9_entry_address, NDS->arm9_size);
	LoadBin((void*)(NDS->arm7_rom_offset + NDS), (void*)NDS->arm7_entry_address, NDS->arm7_size);

	*((vuint32*)0x027FFE24)   = NDS->arm9_entry_address;

	((fp)NDS->arm7_entry_address)();
 
	while (1) ;
	return 0;
}

