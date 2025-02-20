
These are 1.2 MB bootdisk images for Slackware Linux 4.0.0.

These disks use Linux kernel version 2.2.6.

You'll need one of these to get Linux started on your system so that you can
install it. Because of the possibility of collisions between the various Linux
drivers, several bootkernel disks have been provided. You should use the one
with the least drivers possible to maximize your chances of success. All of
these disks support UMSDOS.

You will be using the bootdisk to load and start a rootdisk. See the /rootdsks
directory for these.  (BUT, be sure to read the important information below!)

A bootdisk is created by writing the image to a formatted floppy disk
with RAWRITE.EXE under DOS.  For example, to use RAWRITE.EXE to create the
bare.i bootdisk you'd put a formatted disk in your floppy drive and issue
the following command:

C:\> RAWRITE BARE.I A:

Tip:  If you have no idea which bootdisk to use, start with "bare.i".  This
      is the correct disk to use for most systems with IDE peripherals.

Note:  If you need to support an unusual hardware combination during the
       installation (for example, you want to do an NFS installation using
       a SCSI card and network card that aren't both in the scsinet.s image),
       then you may need to use a generic bootdisk like "bare.i" and load the
       additional drivers as kernel modules before running "setup".  See the
       README files in the ../modules directory for details on how to do this.

--------------------------------------------------------------------------------

...and now some important information (i.e. the bad news ;) for people with
systems that use a 1.2 megabyte (5 1/4 inch) boot floppy drive:

Systems with 1.2 megabyte boot floppy drives are no longer fully supported for
installation.  (if you can get the system installed, everything will work fine)
But, the rootdisks in Slackware no longer fit on 1.2 megabyte floppies.  As a
result, you'll need to use one of the following workarounds.  We will list them
in order of most to least recommended:

1.  If you have a bootable CD-ROM drive, some Slackware CD-ROMs (for example,
    the ones supplied by Walnut Creek CDROM) can be directly booted.  If you
    have this capability, you won't need to use any installation floppies.  

2.  Don't use installation floppies at all.  Instead, start the installation
    process from DOS.  To do this, install loadlin (lodlin16.zip in the kernels
    directory) on your DOS drive, and put these files in the same directory:
    
    A kernel, such as: \kernels\bare.i\zimage
    A rootdisk, such as: \rootdsks\color.gz

    Then, run this command in DOS to start the installation process:

    loadlin zimage root=/dev/ram initrd=color.gz load_ramdisk=1 rw

    If you aren't sure which kernel from the \kernels directory to use (there
    are over 50 of them to choose from), you can take a look at the README
    file in the \bootdsks.144 directory.  The descriptions of those bootdisks
    are equally applicable to the raw kernels found in \kernels.  A good
    starting choice is /kernels/bare.i/zImage, which provides support for IDE
    based machines.

3.  If you happen to have a 1.44 megabyte SECOND floppy drive (B:) you can
    boot one of the bootdisks in this directory, and load the rootdisk from
    your second floppy drive with this command on the boot prompt:

    ramdisk root=/dev/fd1

4.  If you're thinking about using a UMSDOS installation (this puts Linux on
    your DOS drive in a \LINUX directory), consider using the ZipSlack
    installation process to get started.  Instructions are in the \zipslack
    directory.

5.  Install a 1.44 megabyte floppy drive as your first floppy drive.  (and 
    while you're at it, add some more memory ;^)

6.  You can try using one of the unsupported rootdisks in \rootdsks\obsolete
    that will fit on a 1.2 megabyte floppy disk.


--------------------------------------------------------------------------------

Here's a description of the bootdisks:

  These are the bootdisks for IDE systems.  All IDE bootdisks support IDE hard
  drives and CD-ROM drives, plus additional support listed below:


    bare.i             This is the disk to use for installation on most IDE
                       based PCs.  It includes support for IDE/ATAPI CD-ROM
                       drives.
  
    net.i              An IDE/ATAPI bootdisk with Ethernet support.


  The bootdisks listed below are for systems that contain a SCSI controller.
  All SCSI bootdisks feature full IDE hard drive and CD-ROM drive support,
  plus additional SCSI drivers:


    scsi.s             A generic SCSI bootdisk, with support for most SCSI
                       controllers that work under Linux.  (NOTE: This disk
                       wastes a lot of memory, since it contains most of
                       the SCSI drivers.  If you know which SCSI controller
                       your system has, it's *far* better to use the disk 
                       designed especially for it.  But, if you don't know,
                       then this generic disk might just work for you.)

    scsinet.s          Most supported SCSI controllers, plus full ethernet
                       support.



---------------------------------------------------------------------------------
IMPORTANT HELPFUL HINTS: (AND WHAT TO DO IF THE INSTALLED SYSTEM WON'T BOOT)

The kernels provided with the Slackware A series (ide and scsi) are reasonably 
generic to maximize the chances that your system will boot after installation. 
However, you should compile a custom kernel after installing, selecting only the
drivers your system requires.  This will offer optimal performance.  You'll need
to recompile your kernel to enable support for non-SCSI CD-ROM drives, bus-mice,
sound cards, and many other pieces of hardware.  The drivers could not be 
included with the pre-compiled kernels because they cause system hangs and other
compatiblity problems for people that don't have the hardware installed.

On a similar note, any time you use one kernel to install, and a different 
kernel the first time the installed system is started, you run the risk that
the second kernel won't be compatible for some reason. If your system fails
to reboot after installation, you'll have to compile a custom kernel for your
hardware. Follow these steps:

0. If you haven't installed the C compiler and kernel source, do that.

1. Use the bootdisk you installed with to start your machine. At the LILO
   prompt, enter: 
   
     mount root=/dev/hda1
                ^^^^^^^^^ Or whatever your root Linux partition is.

   Ignore any error messages as the system starts up.

2. Log in as root, and recompile the kernel with these steps. (Comments will be
   placed in parenthesis) 

   cd /usr/src/linux
   make config   (Choose your drivers. Repeat this step until you are satisfied
                  with your choices)
   
   If you are using LILO, this will build and install the new kernel:

     make dep ; make clean ; make zlilo 
     rdev -R /vmlinuz 1

   If you are using a bootdisk, these commands will build the kernel and create
   a new bootdisk for your machine:

     make dep ; make clean ; make zImage
     rdev -R zImage 1    (If you use UMSDOS for your root partition, use
                          'rdev -R zImage 0' instead)
     rdev -v zImage -1
     rdev zImage /dev/hda1   (replace /dev/hda1 with the name of your root Linux
                              partition)
     (Now, put a disk into your floppy drive to be made into the new bootdisk:)
     fdformat /dev/fd0u1440 
     cat zImage > /dev/fd0

That should do it! You should now have a Linux kernel that can make full use of
all supported hardware installed in your machine. Reboot and try it out.

Good luck!

---
Patrick Volkerding
volkerdi@slackware.com

PS - Bug reports welcome. Requests for help may be answered if time permits.
     I've been happy to do this in the past, but lately I've had both a lot 
     more work to do and a lot more mail to deal with. It's just not as possible
     to keep up with my mail as it once was.
