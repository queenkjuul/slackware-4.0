/* scsifmt.c -- scsifmt for Iomega ZIP drives running with Linux */

/*  (c) 1996   Alex Butcher (Alex.Butcher@bris.ac.uk) based on the
               work of Grant R. Guenther (grant@torque.net) and
               Itai Nahshon. FORMAT_UNIT mechanism taken from
               FreeBSD's scsifmt.sh. */

/* Compile with: gcc -O2 -o scsifmt scsifmt.c */               
               

/* This program uses SCSI_IOCTL_SEND_COMMAND to deliver the FORMAT_UNIT
   (04h) command to an Iomega ZIP drive. This command should be run before
   running mkfs on a ZIP disk. It will erase ALL information on the disk.
   
   The program attempts to ensure that the SCSI device is actually a ZIP drive
   and that it is not currently mounted.  The checks are *not* foolproof - but
   only root can do these things anyway ! The checks can be disabled by
   commenting #define CHECKZIP. This will be necessary to use this program
   with other kinds of drive (e.g. Iomega JAZ, Syquest etc.), unless you
   alter the comparison string in is_zip(). I would be grateful if you
   could mail alternative _REMOVABLE_ media identifiers to me so that I
   might include them in any future revisions of scsifmt.
   

   Usage:  scsifmt <dev> fmt

   where <dev> must be the full name of a scsi device, eg: /dev/sdc, note
   that you must not specify a partition on the device. 'fmt' must be the
   second parameter (This is an attempt at last minute idiot-proofing!)


   If the disk is in a write protected mode, you will need to use ziptool
   to write enable the disk.

   NB:  The ZIP drive also supports a protection mode 5, under which the
   disk is neither readable nor writable until it is unlocked with a
   password.  This program cannot format a disk in mode 5 - as Linux is
   unable to open a disk that is not readable.  To support this feature
   would require patching the Linux kernel.

   When FORMAT_UNIT finishes, scsifmt ejects the disk to ensure that Linux
   does not use the partition table from before the format.
*/

#include <stdio.h>
#include <mntent.h>
#include <unistd.h>

/* this is ugly and should not be here - but the SCSI include files are
   moving around ...
*/

#define SCSI_IOCTL_SEND_COMMAND 1
#define PAUSE
#define ZIPCHECK

struct sdata {
	int  inlen;
	int  outlen;
	char cmd[256];
} scsi_cmd;

int is_mounted( char * fs )

/* -1 = error,  0 = not mounted,  1 = mounted */

{  struct mntent *mp;
   FILE *mtab;
   
   mtab = setmntent("/etc/mtab","r");
   if (!mtab) return -1;
   while (mp=getmntent(mtab)) 
      if (!strncmp(mp->mnt_fsname,fs,8)) break;
   endmntent(mtab);
   return (mp != NULL);
}

void error(char * msg);

int is_raw_scsi( char * fs )

{	return ((strlen(fs) == 8) && (strncmp("/dev/sd",fs,7) == 0));
}

int	is_zip( int fd )

{	char	id[25];
	int	i;

	scsi_cmd.inlen = 0;
	scsi_cmd.outlen = 40;
	scsi_cmd.cmd[0] = 0x12;		/* inquiry */
	scsi_cmd.cmd[1] = 0;
	scsi_cmd.cmd[2] = 0;
	scsi_cmd.cmd[3] = 0;
	scsi_cmd.cmd[4] = 40;
	scsi_cmd.cmd[5] = 0;

	if (ioctl(fd,SCSI_IOCTL_SEND_COMMAND,(void *)&scsi_cmd))
		error("inquiry ioctl error");

	for(i=0;i<24;i++) id[i] = scsi_cmd.cmd[i+8];
	id[24] = 0;

	return(!strncmp(id,"IOMEGA  ZIP 100",15));
}


void	motor( int fd, int mode )

{	scsi_cmd.inlen = 0;
	scsi_cmd.outlen = 0;
	scsi_cmd.cmd[0] = 0x1b;		/* start/stop */
	scsi_cmd.cmd[1] = 0;
	scsi_cmd.cmd[2] = 0;
	scsi_cmd.cmd[3] = 0;
	scsi_cmd.cmd[4] = mode;
	scsi_cmd.cmd[5] = 0;

	if (ioctl(fd,SCSI_IOCTL_SEND_COMMAND,(void *)&scsi_cmd))
		error("motor control ioctl error");
}

void	unlockdoor( int fd )

{	scsi_cmd.inlen = 0;
	scsi_cmd.outlen = 0;
	scsi_cmd.cmd[0] = 0x1e;		/* prevent/allow media removal */
	scsi_cmd.cmd[1] = 0;
	scsi_cmd.cmd[2] = 0;
	scsi_cmd.cmd[3] = 0;
	scsi_cmd.cmd[4] = 0;
	scsi_cmd.cmd[5] = 0;

	if (ioctl(fd,SCSI_IOCTL_SEND_COMMAND,(void *)&scsi_cmd))
		error("door unlock ioctl error");
}

void eject ( int fd )

{	unlockdoor(fd);
	motor(fd,1);
	motor(fd,2);
}

int	get_prot_mode( int fd )

{      	scsi_cmd.inlen = 0;
	scsi_cmd.outlen = 256;
	scsi_cmd.cmd[0] = 6;		/* Iomega non-sense command */
	scsi_cmd.cmd[1] = 0;
	scsi_cmd.cmd[2] = 2;
	scsi_cmd.cmd[3] = 0;
	scsi_cmd.cmd[4] = 128;
	scsi_cmd.cmd[5] = 0;

	if (ioctl(fd,SCSI_IOCTL_SEND_COMMAND,(void *)&scsi_cmd))
		error("non-sense ioctl error");

	return (scsi_cmd.cmd[21] & 0x0f);	/* protection code */
}

void format(int fd, char *dev)
{
	int s;
	
	s=get_prot_mode(fd);
        if ((s == 2) || (s == 3))
        {
        	printf("scsifmt: %s is write-protected\n",dev);	
	        return;
	}
        
#ifdef PAUSE
	printf("Grace period, last chance to hit INTR (^C)\n");
	printf("Three seconds until format begins.");
	fflush(stdout);
	sleep(1);
	printf(".");
	fflush(stdout);
	sleep(1);
	printf(".");
	fflush(stdout);
	sleep(1);
	printf(" Formatting... this may take a while.\n");
#endif
		
	scsi_cmd.inlen = 0;
	scsi_cmd.outlen = 0;
	scsi_cmd.cmd[0] = 0x04;		/* FORMAT_UNIT */
	scsi_cmd.cmd[1] = 0;
	scsi_cmd.cmd[2] = 0;
	scsi_cmd.cmd[3] = 0;
	scsi_cmd.cmd[4] = 0;
	scsi_cmd.cmd[5] = 0;

	if (ioctl(fd,SCSI_IOCTL_SEND_COMMAND,(void *)&scsi_cmd))
		error("FORMAT_UNIT ioctl error");
		
	eject(fd);
	return;
}


void error(char * msg)

{ 	printf("scsifmt: %s\n",msg);
	exit(1);
}

main(int argc, char **argv)

{  	int rs;
	int zfd;

	if (argc != 3) {
	     	printf("usage: scsifmt </dev/sd?> fmt\n");
		exit(1);
	}
	if (!is_raw_scsi(argv[1])) error("not a raw scsi device");
	rs = is_mounted(argv[1]);
	if (rs == -1) error("unable to access /etc/mtab");
	if (rs == 1) error("device is mounted");

	zfd = open(argv[1],0);
	if (zfd < 0) error("unable to open device");

#ifdef CHECKZIP
	if (!is_zip(zfd)) error("device is not an IOMEGA ZIP drive");
#endif

	if (!strcmp(argv[2],"fmt")) format(zfd, argv[1]);
        else error("unknown command");

	close(zfd);
}
