These are root-install disks for Slackware 4.0 (version 4.0.0)

You will need one of these disks to install Linux. (You'll also need a bootdisk
to be able to boot any of these disks -- see one of the bootdsks directories
for those)  The disk is created by writing the image out with RAWRITE.EXE under
DOS.  For example, to make the color.gz rootdisk, you'd put a formatted floppy
in your floppy drive, and then run this command:

C:\> RAWRITE COLOR.GZ A:

Normally you should not decompress these disks -- the kernel will do that as
it loads them.  The disk images must be written to 1.44MB floppies.  Note
that if you plan to mount the rootdisk instead of loading it into a ramdisk 
(using the method outlined in LOWMEM.TXT for machines with only 4MB) then you
must use one of the smaller "obsolete" images, and must decompress the image
with GZIP.EXE before writing it out to a floppy.


Here's a description of the choices:

--------------------------------------------------------------------------------

 color.gz     This is the default Slackware installation disk, used to install
              Slackware Linux to its own partition.  The name color.gz comes
              from the color menus used to install Slackware.  This is the 
              installation disk most people should use.  NOTE: The 'dialog'
              program used by the install system is not forgiving of extra
              keystrokes entered between screens, so type carefully.

 umsdos.gz    This is similar to the color.gz disk, but installs using UMSDOS,
              a system that allows you to install Slackware into a directory
              on an existing MS-DOS partition.  This isn't as fast as using a
              native Linux filesystem, and can use more drive space because of
              limitations in the way DOS stores small files.  The big advantage
              of using UMSDOS is that you can try Slackware without
              repartitioning your hard drive.  This is much easier for the
              beginner, and a lot less potentially dangerous to your system.

              [NOTE: If you want to use UMSDOS, you might want to consider
              using the ZipSlack installation found in the /zipslack
              directory.  This is a little easier way to get started -- just
              unzip a file on your FAT or FAT32 partition to install Linux. ]

pcmcia.gz     This is a version of the color.gz rootdisk with added support
              for PCMCIA cards.  You should use this disk to install to a
              laptop through the network using NFS, or from a drive connected
              through a PCMCIA card.  [NOTE: at this time there is no support
              for actually installing onto a PCMCIA device]

text.gz       A rootdisk that uses text-based install scripts.  This is
              preferred by some people, but may not have as many features as
              the color.gz, umsdos.gz, or pcmcia.gz rootdisks.  One advantage
              is that you can back up to look at error messages using
              Shift-PageUp.  Another useful feature is the ability to install
              using a serial console with this command on the bootdisk's
              'boot:' prompt:   ramdisk console=ttyS0

There's also this non-installation rootdisk:

rescue.gz:   This is a rescue disk for Linux.  It is a reasonably complete
             mini-Linux system running from a four megabyte ramdisk.  
             It contains an editor (vi), lilo, and other utilities useful for
             fixing your Linux machine if you ever get locked out for some
             reason.  It includes basic TCP/IP utilities such as ping, telnet,
             and ftp.

--------------------------------------------------------------------------------
