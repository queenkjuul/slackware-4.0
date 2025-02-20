From samba!concert!gatech!destroyer!sol.ctr.columbia.edu!usc!snorkelwacker.mit.edu!bloom-picayune.mit.edu!daemon Sun Oct 18 18:24:42 EDT 1992
Article: 13504 of comp.os.linux
Newsgroups: comp.os.linux
Path: samba!concert!gatech!destroyer!sol.ctr.columbia.edu!usc!snorkelwacker.mit.edu!bloom-picayune.mit.edu!daemon
From: Peter MacDonald <pmacdona@tadpole.bcsc.gov.bc.ca>
Subject: Patch to preselect video mode + set ramdisk size
Message-ID: <1992Oct17.073112.13670@athena.mit.edu>
Sender: daemon@athena.mit.edu (Mr Background)
Reply-To: pmacdona@tadpole.bcsc.gov.bc.ca
Organization: The Internet
Date: Sat, 17 Oct 1992 07:31:12 GMT
Lines: 561

Here is my first patch in a good long time.  It does two things.

1) Using a modified version of rdev.c, we can set the startup video
mode of an image.  No more recompiling if you want to change modes.
Example:
		
	vidmode /dev/fd0 1

Here, vidmode is just a link to the new rdev.c pgm enclosed.

2) Set the ramdisk size to nonzero to boot up using a ramdisk.
Loading starts from the 512K mark.  Like so:

	ramsize /dev/fd0 620

Following are the patches, 
Let me know of any problems.

Peter
pmacdona@sanjuan.uvic.ca

-----------------------------------------------------------------
*** boot/setup.S.bak	Fri Oct 16 18:05:07 1992
--- boot/setup.S	Fri Oct 16 23:51:04 1992
***************
*** 282,292 ****
  chsvga:	cld
  	push	ds
  	push	cs
  	pop	ds
  	mov 	ax,#0xc000
  	mov	es,ax
  	lea	si,msg1
- #ifndef SVGA_MODE
  	call	prtstr
  flush:	in	al,#0x60		! Flush the keyboard buffer
  	cmp	al,#0x82
--- 282,297 ----
  chsvga:	cld
  	push	ds
  	push	cs
+ 	mov	ax,[0x01f6]
  	pop	ds
+ 	mov	[0x01f6],ax
+         cmp	ax,#NORMAL_VGA
+ 	je	defvga
+ 	cmp	ax,#0xfffe
+ 	jne	svga
  	mov 	ax,#0xc000
  	mov	es,ax
  	lea	si,msg1
  	call	prtstr
  flush:	in	al,#0x60		! Flush the keyboard buffer
  	cmp	al,#0x82
***************
*** 297,308 ****
  	je	svga			! yes - svga selection
  	cmp	al,#0xb9		! space ?
  	jne	nokey			! no - repeat
! #endif
! #if !defined(SVGA_MODE) || SVGA_MODE == NORMAL_VGA
! 	mov	ax,#0x5019
  	pop	ds
  	ret
- #endif
  svga:	cld
  	lea 	si,idati		! Check ATI 'clues'
  	mov	di,#0x31
--- 302,310 ----
  	je	svga			! yes - svga selection
  	cmp	al,#0xb9		! space ?
  	jne	nokey			! no - repeat
! defvga:	mov	ax,#0x5019
  	pop	ds
  	ret
  svga:	cld
  	lea 	si,idati		! Check ATI 'clues'
  	mov	di,#0x31
***************
*** 552,560 ****
  	call	prtstr
  	pop	si
  	add	cl,#0x80
! #if defined(SVGA_MODE) && SVGA_MODE != NORMAL_VGA
! 	mov	al,#SVGA_MODE		! Preset SVGA mode 
! #else
  nonum:	call	getkey
  	cmp	al,#0x82
  	jb	nonum
--- 554,564 ----
  	call	prtstr
  	pop	si
  	add	cl,#0x80
! 	mov	ax,[0x01f6]
! 	cmp	ax,#0xfffe
! 	je	nonum
! 	cmp	ax,#NORMAL_VGA
! 	jne	gotmode
  nonum:	call	getkey
  	cmp	al,#0x82
  	jb	nonum
***************
*** 566,573 ****
  zero:	sub	al,#0x0a
  nozero:	sub	al,#0x80
  	dec	al
! #endif
! 	xor	ah,ah
  	add	di,ax
  	inc	di
  	push	ax
--- 570,576 ----
  zero:	sub	al,#0x0a
  nozero:	sub	al,#0x80
  	dec	al
! gotmode:	xor	ah,ah
  	add	di,ax
  	inc	di
  	push	ax
*** boot/bootsect.S.bak	Thu Oct 15 22:53:53 1992
--- boot/bootsect.S	Fri Oct 16 19:09:27 1992
***************
*** 425,431 ****
  	.byte 13,10
  	.ascii "Loading"
  
! .org 506
  swap_dev:
  	.word SWAP_DEV
  root_dev:
--- 425,435 ----
  	.byte 13,10
  	.ascii "Loading"
  
! .org 502
! video_mode:
! 	.word 0xFFFE
! ramdisk_size:
! 	.word  0
  swap_dev:
  	.word SWAP_DEV
  root_dev:
*** init/main.c.bak	Fri Oct 16 17:30:22 1992
--- init/main.c	Fri Oct 16 19:15:25 1992
***************
*** 86,91 ****
--- 86,92 ----
  #define EXT_MEM_K (*(unsigned short *)0x90002)
  #define DRIVE_INFO (*(struct drive_info *)0x90080)
  #define SCREEN_INFO (*(struct screen_info *)0x90000)
+ #define RAMDISK_SIZE (*(unsigned short *)0x901F8)
  #define ORIG_ROOT_DEV (*(unsigned short *)0x901FC)
  #define AUX_DEVICE_INFO (*(unsigned char *)0x901FF)
  
***************
*** 145,150 ****
--- 146,152 ----
  struct screen_info screen_info;
  
  unsigned char aux_device_present;
+ int ramdisk_size;
  
  void start_kernel(void)
  {
***************
*** 156,161 ****
--- 158,164 ----
   	drive_info = DRIVE_INFO;
   	screen_info = SCREEN_INFO;
  	aux_device_present = AUX_DEVICE_INFO;
+ 	ramdisk_size = RAMDISK_SIZE;
  	sprintf(term, "TERM=con%dx%d", ORIG_VIDEO_COLS, ORIG_VIDEO_LINES);
  	envp[1] = term;	
  	envp_rc[1] = term;
*** kernel/blk_drv/ramdisk.c.bak	Thu Oct 15 23:19:10 1992
--- kernel/blk_drv/ramdisk.c	Sat Oct 17 00:11:22 1992
***************
*** 6,12 ****
  
  
  #include <linux/config.h>
- #ifdef RAMDISK
  #include <linux/sched.h>
  #include <linux/minix_fs.h>
  #include <linux/fs.h>
--- 6,11 ----
***************
*** 87,93 ****
  {
  	struct buffer_head *bh;
  	struct minix_super_block s;
! 	int		block = 512;	/* Start at block 256 */
  	int		i = 1;
  	int		nblocks;
  	char		*cp;		/* Move pointer */
--- 86,92 ----
  {
  	struct buffer_head *bh;
  	struct minix_super_block s;
! 	int		block = 256;	/* Start at block 256 */
  	int		i = 1;
  	int		nblocks;
  	char		*cp;		/* Move pointer */
***************
*** 138,141 ****
  	printk("\ndone\n");
  	ROOT_DEV=0x0101;
  }
- #endif
--- 137,139 ----
*** kernel/blk_drv/genhd.c.bak	Thu Oct 15 23:21:02 1992
--- kernel/blk_drv/genhd.c	Thu Oct 15 23:22:58 1992
***************
*** 18,23 ****
--- 18,25 ----
  
  static int current_minor = 0;
  extern int *blk_size[];
+ extern int ramdisk_size;
+ 
  /*
   * Create devices for each logical partition in an extended partition.
   * The logical partitions form a linked list, with each entry being
***************
*** 188,196 ****
  	if (nr)
  		printk("Partition table%s ok.\n\r",(nr>1)?"s":"");
  
! #ifdef RAMDISK
! 	rd_load();
! #endif
  	mount_root();
  	return (0);
  }
--- 190,197 ----
  	if (nr)
  		printk("Partition table%s ok.\n\r",(nr>1)?"s":"");
  
! 	if (ramdisk_size)
! 		rd_load();
  	mount_root();
  	return (0);
  }
*** kernel/blk_drv/ll_rw_blk.c.bak	Thu Oct 15 23:21:42 1992
--- kernel/blk_drv/ll_rw_blk.c	Thu Oct 15 23:23:28 1992
***************
*** 18,23 ****
--- 18,24 ----
  #include "blk.h"
  
  extern long rd_init(long mem_start, int length);
+ extern int ramdisk_size;
  
  /*
   * The request-struct contains all necessary data
***************
*** 296,304 ****
  #ifdef CONFIG_BLK_DEV_HD
  	mem_start = hd_init(mem_start,mem_end);
  #endif
! #ifdef RAMDISK
! 	mem_start += rd_init(mem_start, RAMDISK*1024);
! #endif
  	return mem_start;
  }
  
--- 297,304 ----
  #ifdef CONFIG_BLK_DEV_HD
  	mem_start = hd_init(mem_start,mem_end);
  #endif
! 	if (ramdisk_size)
! 		mem_start += rd_init(mem_start, ramdisk_size*1024);
  	return mem_start;
  }
  
*** rdev.c.bak	Sat Oct 17 00:13:52 1992
--- rdev.c	Sat Oct 17 00:13:31 1992
***************
*** 0 ****
--- 1,150 ----
+ /*
+ Date: 11 Mar 92 21:37:37 GMT
+ Subject: rdev - query/set root device
+ From: almesber@nessie.cs.id.ethz.ch (Werner Almesberger)
+ Organization: Swiss Federal Institute of Technology (ETH), Zurich, CH
+ 
+ With all that socket, X11, disk driver and FS hacking going on, apparently
+ nobody has found time to address one of the minor nuisances of life: set-
+ ting the root FS device is still somewhat cumbersome. I've written a little
+ utility which can read and set the root device in boot images:
+ 
+     rdev /dev/at0  (or rdev /linux, etc.) displays the current root device
+     rdev /dev/at0 /dev/hda2         sets it to /dev/hda2
+     sdev /dev/at0 /dev/hda2         sets swapdev to /dev/hda2
+     ramsize /dev/at0 N	            sets ramdisk size to N
+     vidmode /dev/at0 N              sets default video mode to N
+ 
+ rdev accepts an optional offset argument, just in case the address should
+ ever move from 508. If called without arguments, rdev outputs an mtab line
+ for the current root FS, just like /etc/rootdev does.
+ 
+ ramsize sets the size of the ramdisk.  If size is zero, no ramdisk is used.
+ 
+ vidmode sets the default video mode at bootup time.  -1 uses default video
+ mode, -2 uses menu.
+ ram
+ raia - Werner
+    _________________________________________________________________________
+   / Werner Almesberger, ETH Zuerich, CH      almesber@nessie.cs.id.ethz.ch /
+  / IFW A44  Tel. +41 1 254 7213                 almesberger@rzvax.ethz.ch /
+ /_BITNET:_ALMESBER@CZHETH5A__HEPNET/CHADNET:_[20579::]57414::ALMESBERGER_/
+ 
+ */
+ 
+ /* rdev.c  -  query/set root device. */
+ 
+ /* usage: rdev [ boot-image ] [ root-device ] [ offset ] */
+ 
+ #include <stdio.h>
+ #include <string.h>
+ #include <ctype.h>
+ #include <errno.h>
+ #include <fcntl.h>
+ #include <dirent.h>
+ #include <unistd.h>
+ #include <stdlib.h>
+ #include <sys/types.h>
+ #include <sys/stat.h>
+ 
+ 
+ #define DEFAULT_OFFSET 508
+ 
+ 
+ static void die(char *msg)
+ {
+     perror(msg);
+     exit(1);
+ }
+ 
+ 
+ static char *find_dev(int number)
+ {
+     DIR *dp;
+     struct dirent *dir;
+     static char name[PATH_MAX+1];
+     struct stat s;
+ 
+     if (!number) return "Boot device";
+     if ((dp = opendir("/dev")) == NULL) die("opendir /dev");
+     strcpy(name,"/dev/");
+     while (dir = readdir(dp)) {
+ 	strcpy(name+5,dir->d_name);
+ 	if (stat(name,&s) < 0) die(name);
+ 	if ((s.st_mode & S_IFMT) == S_IFBLK && s.st_rdev == number) return name;
+     }
+     sprintf(name,"0x%04x",number);
+     return name;
+ }
+ 
+ 
+ enum { RDEV, SDEV, RAMSIZE, VIDMODE };
+ char *cmdnames[4] = { "rdev", "swapdev", "ramsize", "vidmode" }; 
+ char *desc[4] = { "Root device", "Swap device", "Ramsize", "Video mode" };
+ 
+ int main(int argc,char **argv)
+ {
+     int image,offset,dev_nr, i;
+     char *device, cmd = 0, *ptr, tmp[40];
+     struct stat s;
+ 
+     device = NULL;
+     if (ptr = strrchr(argv[0],'/'))
+     	ptr++;
+     else
+     	ptr = argv[0];
+     for (i=RDEV; i<=VIDMODE; i++)
+     	if (!strcmp(ptr,cmdnames[i]))
+ 	    break;
+     cmd = i;
+     if (cmd>VIDMODE)
+     	cmd=RDEV;
+     offset = DEFAULT_OFFSET-cmd*2;
+ 
+     if  ((cmd==RDEV) && (argc == 1 || argc > 4)) {
+ 	if (stat("/",&s) < 0) die("/");
+ 	printf("%s /\n",find_dev(s.st_dev));
+ 	exit(0);
+     }
+     if ((cmd==RDEV) || (cmd==SDEV))
+     {	
+ 	    if (argc == 4) {
+ 		device = argv[2];
+ 		offset = atoi(argv[3]);
+ 	    }
+ 	    else {
+ 		if (argc == 3) {
+ 		    if (isdigit(*argv[2])) offset = atoi(argv[2]);
+ 		    else device = argv[2];
+ 		}
+ 	    }
+     }
+     else
+     {
+     	if (argc==3)
+ 		device = argv[2];
+     }
+     if (device) {
+     	if ((cmd==SDEV) || (cmd==RDEV))
+ 	{	if (stat(device,&s) < 0) die(device);
+ 	} else
+ 		s.st_rdev=atoi(device);
+ 	if ((image = open(argv[1],O_WRONLY)) < 0) die(argv[1]);
+ 	if (lseek(image,offset,0) < 0) die("lseek");
+ 	if (write(image,(char *)&s.st_rdev,2) != 2) die(argv[1]);
+ 	if (close(image) < 0) die("close");
+     }
+     else {
+ 	if ((image = open(argv[1],O_RDONLY)) < 0) die(argv[1]);
+ 	if (lseek(image,offset,0) < 0) die("lseek");
+ 	dev_nr = 0;
+ 	if (read(image,(char *)&dev_nr,2) != 2) die(argv[1]);
+ 	if (close(image) < 0) die("close");
+ 	printf(desc[cmd]);
+ 	if ((cmd==SDEV) || (cmd==RDEV))
+ 		printf(" %s\n", find_dev(dev_nr));
+ 	else
+ 		printf(" %d\n", dev_nr);
+     }
+     return 0;
+ }
-----------------------------------------------------------------



