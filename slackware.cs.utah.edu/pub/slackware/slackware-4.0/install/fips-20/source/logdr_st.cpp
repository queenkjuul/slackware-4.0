/*
	FIPS - the First nondestructive Interactive Partition Splitting program

	Module logdr_st.cpp

	RCS - Header:
	$Header: c:/daten/fips/source/main/RCS/logdr_str.cpp 1.4 1995/01/19 00:00:54 schaefer Exp schaefer $

	Copyright (C) 1993 Arno Schaefer

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


	Report problems and direct all questions to:

	schaefer@rbg.informatik.th-darmstadt.de
*/

#include <string.h>
#include "types.h"
#include "logdr_st.h"

/* ----------------------------------------------------------------------- */
/* Extract Bios Parameter Block from boot sector                           */
/* ----------------------------------------------------------------------- */

void bios_parameter_block::get (boot_sector *boot_sector)
{
	byte *bp = boot_sector->data;

	memcpy (jump_instruction,bp,3);
	memcpy (oem_name,bp+3,8);
	oem_name[8]=0;
	bytes_per_sector = *(bp+0xb) | (*(bp+0xc) << 8);
	sectors_per_cluster = *(bp+0xd);
	reserved_sectors = *(bp+0xe) | (*(bp+0xf) << 8);
	no_of_fats = *(bp+0x10);
	no_of_rootdir_entries = *(bp+0x11) | (*(bp+0x12) << 8);
	no_of_sectors = *(bp+0x13) | (*(bp+0x14) << 8);
	media_descriptor = *(bp+0x15);
	sectors_per_fat16 = *(bp+0x16) | (*(bp+0x17) << 8);
	sectors_per_track = *(bp+0x18) | (*(bp+0x19) << 8);
	drive_heads = *(bp+0x1a) | (*(bp+0x1b) << 8);
	hidden_sectors = (dword) *(bp+0x1c) | ((dword) *(bp+0x1d) << 8) | ((dword) *(bp+0x1e) << 16) | ((dword) *(bp+0x1f) << 24);
	no_of_sectors_long = (dword) *(bp+0x20) | ((dword) *(bp+0x21) << 8) | ((dword) *(bp+0x22) << 16) | ((dword) *(bp+0x23) << 24);
	bp += 0x24;
	if (sectors_per_fat16 == 0) {
		/* Might be a FAT32 partition */
		sectors_per_fat = (dword)
			(((dword) bp[3] << 24) | ((dword) bp[2] << 16) | (bp[1] << 8) | bp[0]);
		bp += 4;
		flags = (bp[1] << 8) | bp[0]; bp += 2;
		version[0] = *bp++;
		version[1] = *bp++;
		root_cluster = (dword)
			(((dword) bp[3] << 24) | ((dword) bp[2] << 16) | (bp[1] << 8) | bp[0]);
		bp += 4;
		info_sector = ((bp[1] << 8) | bp[0]); bp += 2;
		backup_boot = ((bp[1] << 8) | bp[0]); bp += 2;
		bp += 12; /* Skip over reserved bytes */
	} else {
		sectors_per_fat = sectors_per_fat16;
		root_cluster = 0;
	}
	phys_drive_no = *bp; bp += 2;
	signature = *bp++;
	serial_number = (dword)
		(bp[0] | (bp[1] << 16) | ((dword) bp[2] << 16) | ((dword) bp[3] << 24));
		bp += 4;
	memcpy (volume_label,bp,11); bp += 11;
	volume_label[11] = 0;
	memcpy (file_system_id,bp,8); bp += 8;
	file_system_id[8] = 0;

}

/* ----------------------------------------------------------------------- */
/* Write Bios Parameter Block back into boot sector                        */
/* ----------------------------------------------------------------------- */

void bios_parameter_block::put (boot_sector *boot_sector)
{
	byte *bp = boot_sector->data;

	memcpy (bp,jump_instruction,3);
	memcpy (bp+3,oem_name,8);
	*(bp+0xb) = bytes_per_sector & 0xff;
	*(bp+0xc) = (bytes_per_sector >> 8) & 0xff;
	*(bp+0xd) = sectors_per_cluster;
	*(bp+0xe) = reserved_sectors & 0xff;
	*(bp+0xf) = (reserved_sectors >> 8) & 0xff;
	*(bp+0x10) = no_of_fats;
	*(bp+0x11) = no_of_rootdir_entries & 0xff;
	*(bp+0x12) = (no_of_rootdir_entries >> 8) & 0xff;
	*(bp+0x13) = no_of_sectors & 0xff;
	*(bp+0x14) = (no_of_sectors >> 8) & 0xff;
	*(bp+0x15) = media_descriptor;
	if (sectors_per_fat16 == 0) {
		*(bp+0x16) = 0;
		*(bp+0x17) = 0;
	} else {
		*(bp+0x16) = sectors_per_fat & 0xff;
		*(bp+0x17) = (sectors_per_fat >> 8) & 0xff;
	}
	*(bp+0x18) = sectors_per_track & 0xff;
	*(bp+0x19) = (sectors_per_track >> 8) & 0xff;
	*(bp+0x1a) = drive_heads & 0xff;
	*(bp+0x1b) = (drive_heads >> 8) & 0xff;
	*(bp+0x1c) = hidden_sectors & 0xff;
	*(bp+0x1d) = (hidden_sectors >> 8) & 0xff;
	*(bp+0x1e) = (hidden_sectors >> 16) & 0xff;
	*(bp+0x1f) = (hidden_sectors >> 24) & 0xff;
	*(bp+0x20) = no_of_sectors_long & 0xff;
	*(bp+0x21) = (no_of_sectors_long >> 8) & 0xff;
	*(bp+0x22) = (no_of_sectors_long >> 16) & 0xff;
	*(bp+0x23) = (no_of_sectors_long >> 24) & 0xff;
	bp += 0x24;
	if (sectors_per_fat16 == 0) {
		*bp++ = sectors_per_fat & 0xff;
		*bp++ = (sectors_per_fat >> 8) & 0xff;
		*bp++ = (sectors_per_fat >> 16) & 0xff;
		*bp++ = (sectors_per_fat >> 24) & 0xff;

		*bp++ = flags & 0xff;
		*bp++ = (flags >> 8) & 0xff;
		
		*bp++ = version[0];
		*bp++ = version[1];

		*bp++ = root_cluster & 0xff;
		*bp++ = (root_cluster >> 8) & 0xff;
		*bp++ = (root_cluster >> 16) & 0xff;
		*bp++ = (root_cluster >> 24) & 0xff;

		*bp++ = info_sector & 0xff;
		*bp++ = (info_sector >> 8) & 0xff;
		
		*bp++ = backup_boot & 0xff;
		*bp++ = (backup_boot >> 8) & 0xff;
		
		bp += 12; /* Skip over reserved bytes */
	}
	*bp   = phys_drive_no; bp += 2;
	*bp++ = signature;
	*bp++ = serial_number & 0xff;
	*bp++ = (serial_number >> 8) & 0xff;
	*bp++ = (serial_number >> 16) & 0xff;
	*bp++ = (serial_number >> 24) & 0xff;
	memcpy (bp,volume_label,11); bp += 11;
	memcpy (bp,file_system_id,8); bp += 8;
}

/* ----------------------------------------------------------------------- */
/* Extract some misc. drive parameters from BPB                            */
/* ----------------------------------------------------------------------- */

void logical_drive_info::get (const bios_parameter_block &bpb)
{
	start_fat1 = bpb.reserved_sectors;
	start_fat2 = start_fat1 + bpb.sectors_per_fat;
	start_rootdir = start_fat2 + bpb.sectors_per_fat;
	if (bpb.no_of_rootdir_entries == 0) start_data = start_rootdir;
	else start_data = start_rootdir + (bpb.no_of_rootdir_entries - 1) / 16 + 1;
	if (bpb.sectors_per_cluster == 0) no_of_clusters = 0;
	else {
		no_of_clusters = ((bpb.no_of_sectors ? bpb.no_of_sectors : bpb.no_of_sectors_long) - start_data) / bpb.sectors_per_cluster;
		if (bpb.sectors_per_fat16 == 0) no_of_clusters -= 2;
	}
};

