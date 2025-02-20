Article 13781 of comp.os.linux:
Newsgroups: comp.os.linux
Path: samba!concert!gatech!swrinde!cs.utexas.edu!uunet!mcsun!news.funet.fi!ajk.tele.fi!funic!nntp.hut.fi!nntp!jem
From: jem@snakemail.hut.fi (Johan Myreen)
Subject: Re: 0.98pl1 doesn't recognise my mouse
In-Reply-To: mrd@ecs.soton.ac.uk's message of 20 Oct 92 10:38:36 GMT
Message-ID: <JEM.92Oct20144025@lk-hp-6.hut.fi>
Sender: usenet@nntp.hut.fi (Usenet pseudouser id)
Nntp-Posting-Host: lk-hp-6.hut.fi
Organization: Helsinki University of Technology, Finland
References: <13143@ecs.soton.ac.uk>
Date: 20 Oct 92 12:40:25 GMT
Lines: 189

In article <13143@ecs.soton.ac.uk> mrd@ecs.soton.ac.uk (Mark Dobie) writes:

>I have a Viglen machine with a PS/2 style mouse. At boot up Linux 0.98
>SLS fails to recognise it as any of the mouse types.

Please upgrade to 0.98PL2, or patch your kernel with the patches that
were posted some time ago in comp.os.linux. The PS/2 mouse recognition
doesn't work on some machines running 0.97-0.98PL1.

>hlu's 0.97pl6 root disk used to recognise it as a bus mouse.

It probably didn't. The Microsoft bus mouse recognition was
nonexistent before 0.98, the kernel just happily printed that a
mouse was recognized.

>Obviously I can't run X until linux recognises my mouse. I tried the
>mouse testing program in the X stuff, but I have no idea which device
>my mouse would appear as. It's none of the serial devices.

The PS/2 mouse has major number 10, minor 1. Create the device with

	mknod /dev/psaux c 10 1

PS/2 mouse support for XFree86 is on the way, but not here yet. You'll
need my mconv program to simulate a Microsoft serial mouse. I have
written a new version of the program, which uses a fifo instead of a
pseudo tty. The old version had the disadvantage that you had to
statically specify a pty in Xconfig, a pty that could have been taken
already by some other program.

There might still be some problems with the PS/2 mouse driver,
unfortunately... Please report any problems.

Here's mconv.c. Compile with 'gcc -O -o mconv mconv.c'. You can start
it up in /etc/rc.local, but don't forget to start it in the background
if you do. I first forgot that... :-)

--
Johan Myreen
jem@cs.hut.fi

--- Cut here ---

/*
 * Microsoft serial mouse emulator for PS/2 mouse using a named pipe.
 *
 * This program reads packets from a PS/2 mouse, transforms them 
 * into the corresponding Microsoft serial mouse packets and writes
 * them to a named pipe (fifo). Application programs not supporting
 * the PS/2 mouse protocol (e.g. X11) can then read the packets from
 * the fifo, which to them looks like a serial port with a Microsoft
 * mouse on it.
 *
 * Create a named pipe with suitable permissions with mkfifo,
 * for instance
 *
 *      mkfifo -m 666 /dev/mouse
 * 
 * Make sure you have an entry in /dev for the PS/2 pointing device.
 * If not, create with
 *
 *      mknod /dev/psaux c 10 1
 *
 * Start up the conversion program with:
 *
 *      mconv /dev/psaux /dev/mouse &
 *
 * The program takes two arguments: the real mouse device and the
 * name of the fifo.
 *
 * In Xconfig, fool X into thinking you have a Microsoft mouse
 * on /dev/mouse (or whatever you called it):
 *
 * Microsoft "/dev/mouse"
 * 
 * Johan Myreen
 * jem@cs.hut.fi
 */


#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

static char *fifo_name, *prog_name;
static int fifo, mouse;

void handler()
{
  fprintf(stderr, "\n%s: Mouse killed!\n", prog_name);
  close(fifo);
  close(mouse);
  exit(0);
}

void sigpipe_handler()
{
  close(fifo);
  fifo = open(fifo_name, O_WRONLY);
  if (fifo < 0) {
    fprintf(stderr, "%s: Error reopening fifo.\n", prog_name);
    close(mouse);
    exit(1);
  }
  signal(SIGPIPE, sigpipe_handler);
}
  

unsigned char getbyte(void)
{
  static unsigned char buf[1024];
  static unsigned char *bp = buf, *ep = buf;
  int n;

  if (bp == ep) {
    bp = ep = buf;
    n = read(mouse, buf, 1024);
    if (n>0) {
      ep += n;
    }
  }
  return *bp++;
}


void track_mouse(void)
{
  unsigned char byte1, byte2, byte3, out[3], outbyte;
  int ret;

  while (1) {
    byte1 = getbyte();
    if (byte1 & 0xc0)
      continue;			        /* Resynchronize */
    byte2 = getbyte();
    byte3 = getbyte();
    byte3 = -byte3;
    outbyte = 0x40;
    outbyte |= ((byte2 >> 6)&0x03);
    outbyte |= ((byte3 >> 4)&0x0c);
    outbyte |= (byte1&0x01) << 5;	/* Left button */
    outbyte |= (byte1&0x02) << 3;	/* Right button */
    out[0] = outbyte;
    out[1] = (byte2)&0x3f;
    out[2] = (byte3)&0x3f;
    ret = write(fifo, out, 3);
    if (ret < 0 && errno != EPIPE)
      return;
  }
}


int main(int argc, char **argv)
{

  prog_name = argv[0];
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <mouse dev> <fifo>\n", argv[0]);
    exit(1);
  }

  mouse = open(argv[1], O_RDONLY);

  if (mouse < 0) {
    fprintf(stderr, "%s: error %d opening mouse. Exiting.\n", prog_name, errno);
    close(fifo);
    exit(1);
  }

  fifo_name = argv[2];
  fifo = open(fifo_name, O_WRONLY);

  if (fifo < 0) {
    fprintf(stderr, "%s: error %d opening fifo. Exiting.\n", prog_name, errno);
    exit(1);
  }

  signal(SIGTERM, handler);
  signal(SIGINT, handler);
  signal(SIGHUP, handler);
  signal(SIGPIPE, sigpipe_handler);

  track_mouse();

  fprintf(stderr, "%s: error %d reading or writing.\n", prog_name, errno);
  close(fifo);
  close(mouse);
}


