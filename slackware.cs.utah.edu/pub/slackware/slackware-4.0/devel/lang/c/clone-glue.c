From db@argon.Eng.Sun.COM Mon Jan  2 15:43:49 EST 1995
Article: 24101 of comp.os.linux.development
Path: bigblue.oit.unc.edu!concert!gatech!swrinde!elroy.jpl.nasa.gov!decwrl!koriel!male.EBay.Sun.COM!engnews2.Eng.Sun.COM!argon!db
From: db@argon.Eng.Sun.COM (David Brownell)
Newsgroups: comp.os.linux.development
Subject: Re: Q: multi-thread?
Date: 2 Jan 1995 18:33:32 GMT
Organization: SunSoft, Inc.
Lines: 196
Message-ID: <3e9gts$qqe@engnews2.Eng.Sun.COM>
References: <3dsm50$9m3@engr.orst.edu> <3e1uho$5ml@fido.asd.sgi.com> <3e2dfd$ove@senator-bedfellow.mit.edu> <smcneilD1rM7y.G31@netcom.com>
NNTP-Posting-Host: argon.eng.sun.com

I spent a few hours hacking away to see what this clone() syscall would do.
Here's a simple test program, with comments that should compensate for the
lack of documentation on the kernel call.

In its current incarnation, clone() can't be used as the basis of a full
fidelity POSIX threading implementation.  However, it may be a reasonable
base on which to ** start ** making the LINUX libraries behave correctly
in the face of multiple threads; stuff like errno and stdio for example.

Is there a project under way to make the LINUX libraries MT-safe?

- Dave


/*
 * Reverse-engineered C glue for access to LINUX "clone" system call.  This
 * is basically fork() that shares a bit more data (but not enough to build
 * a complete POSIX threads implementation).
 *
 * Note that clone() is experimental and incomplete.  This file is being
 * constructed to help explore how to implement POSIX threads.  USE AT YOUR
 * OWN RISK, no warrantees expressed or implied, etc.  Matches Fall '94
 * Yggdrasil LINUX (1.1.47+ kernel, see fork.c), your mileage may vary.
 *
 * KERNEL parameters are as follows (and don't match the call signature
 * used outside the kernel, for reasons that will be apparent):
 *
 * - "clone_esp" ... if set to the parent's ESP, causes COPYVM to be set.
 *   In any case, this is the new process's ESP; caller must establish a
 *   valid stack frame there for the clone to return to.
 *
 * - "flags" ... zero or more values logically "or"ed together:
 *	COPYVM ... copies address space (only on write :-).  If not set,
 *		shares the entire address space, but changes made to the
 *		address space itself (e.g. shmat, munmap) are not shared.
 *
 *	COPYFD ... copies all open file descriptors.  If not set, shares
 *		individual table entries, but won't pick up changes to the
 *		file descriptor table itself (e.g. open, close).
 *
 *	"SIGNAL" ... the number of a signal that will be sent to the parent
 *		when the cloned process dies.  If set to zero, wait4() must
 *		pass __WCLONE to wait for clones.
 *
 * The "CLONEVM" code (COPYVM clear) is noted as incomplete/wrong.  Races may
 * exist, and it doesn't treat SysV SHM the way it treats other memory.
 *
 * For the purpose of using clone() to implement POSIX threads, clones need
 * to share the entire address space so that modifications (mprotect, sbrk,
 * mmap, shmat, ...) made by one clone (or parent) are seen by all others
 * immediately.  Otherwise, functions and data won't be uniformly accessible
 * to all threads.
 *
 *
 * Similarly, the "SHAREFD" code (COPYFD clear) seems a bit odd ... nobody
 * uses COPYFD (that I could easily find), and it doesn't quite seem to do
 * what was originally intended.
 *
 * For the purpose of implementing POSIX threads, the clones and parents need
 * to share the file table so that changes made by one are seen by the others.
 * Neither "CLONEFD" nor "SHAREFD" has the right semantics.
 *
 *
 * NOTE:  There's all kinds of process state that really ought to be shared
 * between "clones" in order to support POSIX threads.  Without looking at
 * POSIX specs (just some LINUX source) this would seem to include at least:
 *
 *	- address space (as noted above)
 *	- file descriptors (as noted above)
 *	- cwd and root inodes
 *	- signal handling
 *	- control TTY
 *	- SysV IPC (SHM, SEM, MSGQ)
 *	- real and effective UIDs
 *	- ... and more
 *
 * It's this required degree of sharing that led at least one OS to make
 * the kernel-supported thread context be distinct from processes; the
 * threads share all that stuff via the proceses to which they're tied.
 * The kernel thread context is what's involved in scheduling decisions,
 * and POSIX threading uses a "create kernel thread" style primitive that
 * shares all the process context, not a fork() variant that's got to be
 * taught on an item-by-item basis what needs to be shared.
 *
 * SUGGESTION:  There should be a new kernel data structure representing
 * the kernel portion of a POSIX thread.  This includes fields from the
 * LINUX "task_struct" (in <linux/sched.h>) and would have fields like
 *
 *	- tss (for per-thread context, from task_struct)
 *	- kernel stack page (from task_struct)
 *	- back pointer to task_struct (the 'process')
 *
 * One such structure could be preallocated in each "task_struct"; it'd be
 * useful for smooth kernel evolution if nothing else.  Threaded processes
 * would have a set of such structures; unthreaded ones, just the single one.
 * Scheduling would need to work in terms of those structures rather than
 * today's "task_struct".  All accesses (!) to today's "task_struct" would
 * need to indirect to it; "current" might be a macro (curthread->process).
 */

#include	<syscall.h>
#include	<unistd.h>

#include	<sys/types.h>

/*
 * XXX this calling convention is bogus -- no MT-oriented calls
 * should use global state such as errno.
 */
pid_t
clone (
    void	(*func)(void *ctx),	/* function for clone to call */
    void	*ctx,			/* parameter to call it with */
    void	*stack,			/* stack to use */
    size_t	stack_len,		/* size thereof, in bytes */
    int		lwp_flags		/* flags (UNUSED) */
)
{
    void	**esp = (void **) ((((long)stack) + stack_len) & ~0x03);
    pid_t	retval;

    /*
     * Make a stack that unwinds nicely enough ... syscall returns
     * to the "func" that the clone is to call, with its parameter
     * in normal position.  If the clone returns, it exits.
     */
    *--esp = 0;				/* parameter to _exit */
    *--esp = ctx;			/* parameter to func */
    *--esp = (void *) _exit;		/* return address for "func" */

    *--esp = func;			/* "return" address for clone */
    *--esp = 0;				/* params popped by syscall */
    *--esp = 0;
    *--esp = 0;

    return syscall (SYS_clone, esp, /* clone_flags */ 0, 0, 0, 0);
}


/*
 * Some sample code to test _clone() ...
 */

#include	<signal.h>
#include	<stdio.h>
#include	<string.h>

#include	<linux/sched.h>

static void
clone_call (void *param)
{
    char *str = (char *)param;

    /*
     * RACE -- runtime libraries are not yet reentrant
     * So we emit our message and exit without cleanup.
     */
    printf ("%s, I'm clone %d!\n", str, getpid ());
    fflush (stdout);
}

static char	new_stack [4096];

int
main ()
{
    pid_t	clone_pid;

    clone_pid = clone (clone_call, "hello world",
		    new_stack, sizeof new_stack,
		    0);

    switch (clone_pid) {
      case -1:
	perror ("clone");
	return -1;

      case 0:
        puts ("?? Parent is the clone ??");
        return -1;

      default:
	/*
	 * Runtime libraries are not yet reentrant, we've got
	 * problems doing the most basic stuff, like having
	 * both clones use STDIO.
	 */
	_exit (0);
    }
}
-- 
David Brownell                        db@Eng.Sun.COM.
Distributed Object Management

main(a){printf(a,34,a="main(a){printf(a,34,a=%c%s%c,34);}",34);}


