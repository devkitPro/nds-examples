//---------------------------------------------------------------------------------
#ifndef	_nds_file_h_
#define	_nds_file_h_
//---------------------------------------------------------------------------------



typedef struct t_NDS_header
{
    char title[0xC];
    char gamecode[0x4];
    unsigned char makercode[2];
    unsigned char unitcode;
    unsigned char devicetype;       // type of device in the game card
    unsigned char devicecap;        // capacity
    unsigned char reserved1[0x9];
    unsigned char romversion;
    unsigned char reserved2;
    unsigned int arm9_rom_offset;
    unsigned int arm9_entry_address;
    unsigned int arm9_ram_address;
    unsigned int arm9_size;
    unsigned int arm7_rom_offset;
    unsigned int arm7_entry_address;
    unsigned int arm7_ram_address;
    unsigned int arm7_size;
    unsigned int fnt_offset;
    unsigned int fnt_size;
    unsigned int fat_offset;
    unsigned int fat_size;
    unsigned int arm9_overlay_offset;
    unsigned int arm9_overlay_size;
    unsigned int arm7_overlay_offset;
    unsigned int arm7_overlay_size;
    unsigned char rom_control_info1[8];
    unsigned int icon_title_offset;
    unsigned short secure_area_crc;
    unsigned short rom_control_info2;
    unsigned int offset_0x70;
    unsigned int offset_0x74;
    unsigned int offset_0x78;
    unsigned int offset_0x7C;
    unsigned int application_end_offset;            // rom size
    unsigned int rom_header_size;
    unsigned int offset_0x88;
    unsigned int offset_0x8C;
    unsigned int offset_0x90;
    unsigned int offset_0x94;
    unsigned int offset_0x98;
    unsigned int offset_0x9C;
    unsigned int offset_0xA0;
    unsigned int offset_0xA4;
    unsigned int offset_0xA8;
    unsigned int offset_0xAC;
    unsigned int offset_0xB0;
    unsigned int offset_0xB4;
    unsigned int offset_0xB8;
    unsigned int offset_0xBC;
    unsigned char logo[156];
    unsigned short logo_crc;
    unsigned short header_crc;
    unsigned int offset_0x160;
    unsigned int offset_0x164;
    unsigned int offset_0x168;
    unsigned int offset_0x16C;
    unsigned char zero[0x90];
}	NDS_header;

//---------------------------------------------------------------------------------
#endif	//_nds_file_h_
//---------------------------------------------------------------------------------
