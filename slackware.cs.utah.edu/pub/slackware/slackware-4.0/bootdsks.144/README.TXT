These are 1.44 MB bootdisk images for Slackware Linux 4.0.0.

These disks use Linux kernel version 2.2.6.

You'll need one of these to get Linux started on your system so that you can
install it.  Because of the possibility of collisions between the various Linux
drivers, several bootkernel disks have been provided.  You should use the one
with the least drivers possible to maximize your chances of success.  

You will be using the bootdisk to load and start a rootdisk.  See the /rootdsks
directory for these.

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

Here's a description of the disks:

 These are the bootdisks for IDE based systems.  All IDE bootdisks support
 IDE hard drives and CD-ROM drives, plus additional support listed below.


    aztech.i           CD-ROM drives:  Aztech CDA268-01A, Orchid CD-3110,
                       Okano/Wearnes CDD110, Conrad TXC, CyCDROM CR520, CR540.

    bare.i             This is the disk to use for installation on most IDE
                       based PCs.  It includes support for IDE/ATAPI CD-ROM
                       drives.  Most CD-ROM drives made today fall into this
                       catagory.
  
    bareapm.i          This disk is the same as the bare.i image, with the
                       addition of Advanced Power Management support.  This
                       is usually used on laptops to check the battery
                       level, or to put the machine in standby or sleep mode.
                       On machines that aren't laptops, it can cause booting
                       problems.  This is the correct disk to use for a PCMCIA
                       install.

    cdu31a.i           Sony CDU31/33a CD-ROM.

    cdu535.i           Sony CDU531/535 CD-ROM.

    cm206.i            Philips/LMS cm206 CD-ROM with cm260 adapter card.

    goldstar.i         Goldstar R420 CD-ROM  (sometimes sold in a 'Reveal
                       Multimedia Kit').
   
    isp16.i            Supports ISP16/MAD16/Mozart CD-ROM drives.
                       Boot time command line options (or 'append=' options
                       in /etc/lilo.conf) are:
 
                       isp16=<port>,<irq>,<dma>,<drive_type>

                       Valid values for drive_type include: Sanyo, Panasonic
                       (same as Sanyo), Sony and Mitsumi.  Default values are:
                       port=0x340, irq=0, dma=0, drive_type=Sanyo.

    lowmem.i           This is a really stripped-down Linux kernel which might
                       be useful for installing on IDE systems with a low
                       amount of RAM (less than 8MB).  If bare.i runs into
                       problems, you might try this.  NOTE:  On systems with
                       extremely low memory (4MB), ZipSlack plus the
                       fourmeg.zip add-on (found in the zipslack directory)
                       may boot and run even in cases where lowmem.i doesn't.

    mcd.i              NON-IDE Mitsumi CD-ROM support.

    mcdx.i             Improved NON-IDE Mitsumi CD-ROM support.

    net.i              An IDE/ATAPI bootdisk with Ethernet support.  Use this
                       for installing over a network to an IDE hard drive.

                       NOTE:  net.i, while a nice shortcut if it works, does
                       not work on every machine, nor does it include every
                       Linux network driver.  Because of the large number of
                       devices it looks for, it can hang the machine or cause
                       other problems if it misidentifies something.  This is
                       an unavoidable risk when probing for hardware on the PC
                       platform.

                       If you run into problems with net.i (or it doesn't see
                       your network card), don't panic, just use a disk with
                       less drivers.  If you're not installing over the
                       network, bare.i is probably the right choice.  If you
                       need network support, use bare.i, plus put 'insmod' and
                       the required network modules on another floppy to load
                       before running setup.  The README files in the 
                       ../modules directory explain this process in detail.

                       This disk also includes PLIP support, but you'll need
                       to pass a parameter to the kernel to tell the parallel
                       port driver which IRQ to use.  For example, if you want
                       to use a parallel port on 0x378, IRQ 7, you would use
                       this command on the bootdisk's 'boot:' prompt:

                       ramdisk parport=0x378,7

                       You may also use this command to make the driver
                       autodetect your port and IRQ:

                       ramdisk parport=auto

    no_kbd.i           This version of bare.i is a workaround for a problem:
                       Some newer P2/Celeron systems have been known to 
                       reboot themselves as the kernel is starting up.  This
                       seems to happen as the keyboard LEDs are reset.  The
                       no_kbd.i does not reset the keyboard LEDs, and will 
                       boot correctly on most machines with this symptom.
                       A BIOS upgrade from the motherboard manufacturer may
                       fix the problem.

    no_pci.i           If you're having problems with a hang during PCI
                       probing, try this IDE-supporting bootdisk without PCI
                       BIOS support.  Some old PCI motherboards have BIOS
                       bugs and may crash if "PCI bios support" is enabled,
                       but they run fine without this option.  In many cases
                       the BIOS can be fixed with a flash update from the
                       motherboard manufacturer's web site.
 
    optics.i           Optics Storage 8000 AT CD-ROM (the 'DOLPHIN' drive).

    pportide.i         This is an extended version of bare.i will support for
                       a wide variety of parallel-port IDE devices.  Supports
                       parallel-port products from MicroSolutions, 
                       Hewlett-Packard, SyQuest, Imation, Avatar, and other
                       manufacturers.

    sanyo.i            Sanyo CDR-H94A CD-ROM support.

    sbpcd.i            Matsushita, Kotobuki, Panasonic, CreativeLabs 
                       (Sound Blaster), Longshine and Teac NON-IDE CD-ROM
                       support.  IMPORTANT!  I can't possibly stress enough
                       that this disk is *not* for IDE/ATAPI CD-ROM drives,
                       which include nearly all of the drives made by these
                       manufacturers recently.  For IDE/ATAPI CD-ROM drives,
                       use bare.i.

    smp.i              This disk is the same as the generic bare.i bootdisk,
                       but adds support for motherboards with multiple CPUs.
                       (symmetric multiprocessing, or SMP)

    xt.i               MFM (very very old) hard drive support.


  The bootdisks listed below are for systems that contain a SCSI controller.
  All SCSI bootdisks feature full IDE hard drive and CD-ROM drive support,
  plus additional SCSI drivers.  


    7000fast.s         Western Digital 7000FASST SCSI support.

    acard.s            ACARD 870U/W SCSI host adapter support.

    advansys.s         This bootdisk supports all AdvanSys SCSI controllers,
                       including some SCSI cards included with HP CD-R/RW 
                       drives, the Iomega Jaz Jet SCSI controller, and the 
                       SCSI controller on the Iomega Buz multimedia adapter.

    aha152x.s          Adaptec 152x SCSI support.  This supports the AHA-1510,
                       AHA-1520, AHA-1522, and AHA-2825 SCSI host adapters.

    aha1542.s          Adaptec 1542 SCSI support.

    aha1740.s          Adaptec 1740 SCSI support.

    aic7xxx.s          Adaptec AIC7xxx SCSI support.  These include the 274x
                       EISA cards, 284x VLB cards, 294x PCI cards, 394x PCI 
                       cards, 3985 PCI card, and several versions of the
                       Adaptec built-in SCSI controllers on various PC
                       motherboards.

    am53c974.s         AM53/79C974 SCSI support.

    buslogic.s         This supports BusLogic MultiMaster and FlashPoint SCSI
                       controllers.

    dtc3280.s          DTC (Data Technology Corp) 3180/3280 SCSI support.

    eata_dma.s         DPT EATA-DMA SCSI support.  (Boards such as PM2011, 
                       PM2021, PM2041, PM3021, PM2012B, PM2022, PM2122, PM2322,
                       PM2042, PM3122, PM3222, PM3332, PM2024, PM2124, PM2044, 
                       PM2144, PM3224, PM3334.)

    eata_isa.s         DPT EATA-ISA/EISA SCSI support.  (Boards such as 
                       PM2011B/9X, PM2021A/9X, PM2012A, PM2012B, PM2022A/9X,
                       PM2122A/9X, PM2322A/9X)

    eata_pio.s         DPT EATA-PIO SCSI support.  (PM2001 and PM2012A)

    fdomain.s          This supports Future Domain's 16-bit SCSI host adapters
                       (TMC-1660/1680, TMC-1650/1670, TMC-3260, 
                       TMC-1610M/MER/MEX) and other adapters based on the
                       Future Domain chipsets (Quantum ISA-200S, ISA-250MG;
                       Adaptec AHA-2920; and at least one IBM board).

    gdt.s              This is a bootdisk for all SCSI Disk Array Controllers
                       (EISA/ISA/PCI) manufactured by ICP Vortex.

    ibmmca.s           This is a bootdisk based on a development kernel which
                       supports MicroChannel Architecture, found in some IBM 
                       PS/2 machines and laptops. It is a bus system similar to
                       PCI or ISA.  Support for most MCA SCSI, Ethernet, and
                       Token Ring adapters is included.

    in2000.s           Always IN2000 SCSI support.

    initio.s           Supports the Initio 91XXU(W) and Initio 91XXU(W) SCSI
                       host adapters.

    iomega.s           IOMEGA parallel port SCSI support.  This supports
                       the parallel-port Zip drives as a Linux SCSI device.
                       This disk supports the older PPA parallel Zip drives
                       as well as the newer ZipPlus drives that use the IMM
                       driver.

    megaraid.s         This bootdisk supports the AMI MegaRAID 428 and 438
                       (and maybe 466) SCSI host adapters.

    n53c4xx.s          NCR 53c406a and Symbios Logic sym53c416 SCSI support.

    n_5380.s           NCR 5380 and 53c400 SCSI support.  This is the generic
                       NCR family of SCSI controllers, not to be confused with
                       the NCR 53c7 or 8xx controllers.  An example of a
                       controller that uses the NCR53c400 chip is the Trantor
                       T130B.

    n_53c7xx.s         NCR 53c7xx, 53c8xx SCSI support.  (Most NCR PCI
                       SCSI controllers use this driver, or the driver below)

    n_53c8xx.s         This is the FreeBSD NCR driver adapted to Linux for
                       the NCR53C8XX family of PCI-SCSI controllers. This
                       driver supports parity checking, tagged command queuing,
                       fast SCSI II transfer up to 10 MB/s with narrow SCSI
                       devices and 20 MB/s with wide scsi devices.  It also
                       supports Ultra SCSI data transfers with NCR53C860 and
                       NCR53C875 controllers.  The following Symbios/NCR 
                       chipsets are supported by the driver in this bootdisk:
                       810 810A 815 825 825A 860 875 876 895 896

    no_kbd.s           This version of scsi.s is a workaround for a problem:
                       Some newer P2/Celeron systems have been known to
                       reboot themselves as the kernel is starting up.  This
                       seems to happen as the keyboard LEDs are reset.  The
                       no_kbd.s does not reset the keyboard LEDs, and will
                       boot correctly on most machines with this symptom.
                       A BIOS upgrade from the motherboard manufacturer may
                       fix the problem.

    no_pci.s           If you're having problems with a hang during PCI
                       probing, try this SCSI-supporting bootdisk without PCI
                       BIOS support.  Some old PCI motherboards have BIOS
                       bugs and may crash if "PCI bios support" is enabled,
                       but they run fine without this option.  In many cases
                       the BIOS can be fixed with a flash update from the
                       motherboard manufacturer's web site.

    old_cd.s           This disk is provided in the unlikely event that
                       someone needs to install from an old CD-ROM drive on a
                       proprietary (*not* SCSI or IDE/ATAPI) interface to a
                       hard drive on a SCSI controller.  This disk supports
                       most Linux supported SCSI controllers, plus the CD-ROM
                       drives supported by these bootdisks:  aztech.i, cdu31a.i,
                       cdu535.i, cm206.i, goldstar.i, isp16.i, mcd.i, mcdx.i,
                       optics.i, sanyo.i, and sbpcd.i.
                       
    pas16.s            Pro Audio Spectrum/Studio 16 SCSI support.

    pci2000.s          This bootdisk supports the PCI2000I, PCI2220i, and 
                       PSI240i EIDE interface cards.  All of these act as
                       SCSI host adapters.

    qlog_fas.s         ISA/VLB/PCMCIA Qlogic FastSCSI! support.  (also 
                       supports the Control Concepts SCSI cards based on the
                       Qlogic FASXXX chip)

    qlog_isp.s         Supports all Qlogic PCI SCSI controllers, except the
                       PCI-basic, which is supported by the AMD SCSI driver
                       found on the am53c974.s bootdisk.

    scsi.s             A generic SCSI bootdisk, with support for most SCSI
                       controllers that work under Linux.  (NOTE: This disk
                       wastes a lot of memory, since it contains nearly *all*
                       of the SCSI drivers.  If you know which SCSI controller
                       your system has, it's *far* better to use the disk 
                       designed especially for it.  But, if you don't know,
                       then this generic disk might just work for you.)

    scsinet.s          Supports most SCSI controllers, plus many of the most
                       common ethernet cards.  Use this for installing over a
                       network to a SCSI drive.  (Or you can also use the
                       bootdisk for your SCSI controller plus the appropriate
                       ethernet module.  See the README files in the ../modules
                       directory if you want to try this approach)
                       NOTE:  Don't be too surprised if scsinet.s causes your
                       machine to hang.  It's loaded with nearly every SCSI
                       and network driver for Linux, and unless your hardware
                       is well behaved, the probing process can freeze your
                       machine.  If that happens, don't panic, just use a
                       disk with less drivers.  If you're not installing over
                       the network, the disk designed for your SCSI controller
                       is the right choice.  If you need network support, use
                       the disk designed for your SCSI card, plus put 'insmod'
                       and the required network modules on another floppy to
                       load before running setup.  The README files in the
                       ../modules directory explain this process in detail.

    seagate.s          Seagate ST01/ST02, Future Domain TMC-885/950 SCSI
                       support.

    sgivis.s           This bootdisk supports the SGI Visual Workstation
                       models 320 and 540.  The kernel also supports Adaptec
                       aic7xxx SCSI cards, multiple CPUs, and the SGI DBE 
                       video frame buffer graphics console.

    smp.s              This disk is the same as the generic scsi.s bootdisk,
                       but adds support for motherboards with multiple CPUs.
                       (symmetric multiprocessing, or SMP)

    sym538xx.s         This bootdisk contains enhanced support for Symbios/NCR
                       SCSI chipsets in the SYM53C8XX family.  It drops support
                       of some of the oldest chipsets in this family in order
                       to use some new features, so older cards will need to
                       use the n_53c7xx.s or n_53c8xx.s bootdisk images instead.
                       The driver in this bootdisk supports the following
                       Symbios/NCR chipsets: 810A 825A 860 875 876 895 896

    tekram.s           This bootdisk supports the Tekram DC390(T) PCI SCSI
                       Hostadapter with the Am53C974A chip, and perhaps other
                       cards using the same chip.  It does _not_ support the
                       DC390W/U/F adaptor with the NCR/Symbios chips -- for
                       those cards, use the n_53c8xx.s bootdisk.

    trantor.s          Trantor T128/T128F/T228 SCSI support.

    ultrastr.s         UltraStor 14F, 24F, and 34F SCSI support.

    ustor14f.s         UltraStor 14F and 34F SCSI support.



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
the second kernel won't be compatible for some reason.  If your system fails
to reboot after installation, you'll have to compile a custom kernel for your
hardware.  Follow these steps:

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

That should do it!  You should now have a Linux kernel that can make full use of
all supported hardware installed in your machine.  Reboot and try it out.

Good luck!

---
Patrick Volkerding
volkerdi@slackware.com

PS - Bug reports welcome.  Requests for help may be answered if time permits.
     I've been happy to do this in the past, but lately I've had both a lot 
     more work to do and a lot more mail to deal with.  It's just not as 
     possible to keep up with my mail as it once was.
