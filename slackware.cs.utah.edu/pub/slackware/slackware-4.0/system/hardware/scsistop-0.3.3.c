/*
 * scsistop.c -- START/STOP UNIT for scsi-drives running with Linux
 * Version: 0.3.3
 * Date:    1998/06/01
 */

/*
 * Modified by Christoph Conrad (Christoph.Conrad@post.rwth-aachen.de)
 * based on a source by
 * (c) 1996   Alex Butcher (Alex.Butcher@bris.ac.uk) based on the
 *            work of Grant R. Guenther (grant@torque.net) and
 *            Itai Nahshon. FORMAT_UNIT mechanism taken from
 *            FreeBSD's scsistop.sh.
 * (scsifmt.c -- scsifmt for Iomega ZIP drives running with Linux)
 *
 *
 * Compile with: gcc -O2 -o scsistop scsistop.c
 *
 * History:
 *
 * 11.10.1998 V0.3.3
 * - eliminated all compiler-warnings (mainly added some #includes)
 * 01.06.1998 V0.3.1
 * - first public version
 */

/***********************************************************************
 * INCLUDES
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <mntent.h>
#include <unistd.h>


/***********************************************************************
 * DEFINES
 ***********************************************************************/

/* Check for a specific drive (via scsi-command 'inquiry') */
/*
 * #define CHECK_DRIVE
 * #define DRIVE_SPEC "IOMEGA  ZIP 100"
 */

/*
 * this is ugly and should not be here - but the SCSI include files are
 * moving around ...
 */

#define SCSI_IOCTL_SEND_COMMAND 1


/***********************************************************************
 * STRUCTS
 ***********************************************************************/

struct sdata
{
    int  inlen;
    int  outlen;
    char cmd[256];
} scsi_cmd;


/***********************************************************************
 * FUNCTIONS
 ***********************************************************************/

/***********************************************************************
 * NAME: error
 *
 * ARGUMENTS:
 * msg: error-message
 *
 * PURPOSE:
 * outputs errormessage msg and terminates program
 *
 * RETURNS:
 * %
 */
void error(char * msg)

{
    printf("scsistop: %s\n",msg);
    exit(1);
}


/***********************************************************************
 * NAME: is_mounted
 *
 * ARGUMENTS:
 * fs: string describing scsi-device
 *
 * PURPOSE:
 * looks in mtab whether the device is currently mounted
 *
 * RETURNS:
 * -1 = error
 *  0 = not mounted
 *  1 = mounted
 */
int is_mounted( char * fs )
{
    struct mntent *mp;
    FILE *mtab;

    mtab = setmntent("/etc/mtab","r");
    if ( ! mtab )
	return -1;

    while ( 0 != ( mp = getmntent( mtab ) ) )
	if ( ! strncmp( mp->mnt_fsname, fs, 8 ) )
	    break;

    endmntent(mtab);

    return ( mp != NULL );
}


/***********************************************************************
 * NAME: is_raw_scsi
 *
 * ARGUMENTS:
 * fs: string describing device
 *
 * PURPOSE:
 * Checks whether the drive-spec is "/dev/sd." (. == any character)
 *
 * RETURNS:
 * 1: ok
 * 0: doesnt match
 */
int is_raw_scsi( char * fs )
{
    return ((strlen(fs) == 8) && (strncmp("/dev/sd",fs,7) == 0));
}


#if CHECK_DRIVE
/***********************************************************************
 * NAME: is_drive_spec
 *
 * ARGUMENTS:
 * fd:  scsi-device file descriptor
 * str: string describing the device
 *
 * PURPOSE:
 * compares argument str with devicedescription as returned by scsi-command
 * 'inquiry'
 *
 * RETURNS:
 *  0 = drive isnt expected one
 *  1 = drive has name as expected
 */
int is_drive_spec( int fd, char * str )
{
    char	id[25];
    int	i;

    scsi_cmd.inlen  = 0;
    scsi_cmd.outlen = 40;
    scsi_cmd.cmd[0] = 0x12;		/* inquiry */
    scsi_cmd.cmd[1] = 0;
    scsi_cmd.cmd[2] = 0;
    scsi_cmd.cmd[3] = 0;
    scsi_cmd.cmd[4] = 40;
    scsi_cmd.cmd[5] = 0;

    if (ioctl(fd,SCSI_IOCTL_SEND_COMMAND,(void *)&scsi_cmd))
	error("inquiry ioctl error");

    for(i=0;i<24;i++)
	id[i] = scsi_cmd.cmd[i+8];

    id[24] = 0;

    printf ( "%s\n", id );

    return(!strncmp(id,str,15));
}
#endif



/***********************************************************************
 * NAME: main
 *
 * ARGUMENTS:
 * usage: scsistop </dev/sd?> <start|stop>\n
 *
 * PURPOSE:
 * does 'START UNIT' or 'STOP UNIT' for scsi-device if it is not currently
 * mounted
 *
 * RETURNS:
 *  0 = command succeded
 *  1 = error
 */
int main(int argc, char **argv)
{
    int  rs;
    int  zfd;
    char buffer[100];

    if ( argc != 3 )
    {
	printf("usage: scsistop </dev/sd?> <start|stop>\n");
	exit(1);
    }

    if ( ! is_raw_scsi( argv[1] ) )
	error("not a raw scsi device");

    rs = is_mounted( argv[1] );
    if (rs == -1)
	error("unable to access /etc/mtab");
    if (rs == 1)
    {
	error("device is mounted");
    }

    zfd = open( argv[1], 0 );
    if (zfd < 0)
	error("unable to open device");

#ifdef CHECK_DRIVE
    if ( ! is_drive_spec ( zfd, DRIVE_SPEC ) )
	error("device is not an IOMEGA ZIP drive");
#endif

    scsi_cmd.inlen  = 0;
    scsi_cmd.outlen = 0;
    scsi_cmd.cmd[0] = 0x1b;     /* start/stop   */
    scsi_cmd.cmd[1] = 0;        /* ! lun << 5   */
    scsi_cmd.cmd[2] = 0;
    scsi_cmd.cmd[3] = 0;
    scsi_cmd.cmd[5] = 0;

    if ( ! strcmp ( argv[2], "start" ) )
    {
	scsi_cmd.cmd[4] = 1;
    }
    else if ( ! strcmp ( argv[2], "stop" ) )
    {
	/* sync device */
	strcpy ( buffer, "sync " );
	strcat ( buffer, argv[1] );
	printf ( "Syncing %s...\n", buffer );
	if ( system( buffer ) != 0 )
	{
	    error("sync failed!");
	}

	scsi_cmd.cmd[4] = 0;
    }
    else
    {
	error ( "usage: scsistop </dev/sd?> <start|stop>\n" );
    }


    if ( ioctl( zfd, SCSI_IOCTL_SEND_COMMAND, (void *) & scsi_cmd) )
	error("motor control ioctl error");

    close(zfd);

    return 0;
}
