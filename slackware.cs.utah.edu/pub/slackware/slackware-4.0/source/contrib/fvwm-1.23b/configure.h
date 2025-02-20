#define FVWMDIR "/usr/lib/X11/fvwm"
#define FVWM_ICONDIR "/usr/include/X11/bitmaps:/usr/include/X11/pixmaps"
#define FVWMRC  "/usr/lib/X11/fvwm/system.fvwmrc"
#define FVWMFILEMGRRC  "/usr/lib/X11/fvwm/system.fvwmfmgr"

/*  linker flags needed to locate and link in the Xpm library, if you use it */
#define XPMLIBRARY -L/usr/lib/X11 -lXpm
/* #define XPMLIBRARY /usr/lib/X11/libXpm.a */

/* Imake command needed to put modules in desired target location */
/* Use the second version if it causes grief */
#define TARGET_DIR BINDIR=FVWMDIR 
/* #define TARGET_DIR*/


/* If you want Imake to leave your binary in the standard place where
 * Imake wants to leave binaries, then choose the second line here.
 * If you want to install it in a different directory, uncomment and
 * edit the first line */
/*#define FVWM_BIN_DIR BINDIR=/local/homes/dsp/nation/sun4/bin*/
#define FVWM_BIN_DIR

/* Compiler over-ride for Imakefiles */
/* Leave it as shown to get your default compiler */
#define COMPILER CC=gcc -Wall 
/* #define COMPILER */


/*
 * Pick and choose your compile time options.
 * There are way too many compile time options! 
 * Type in your choices at the line EFLAGS=...
 * Here's a summary of the choices:
 **************************************#
 * Major Features
 **************************************#
 *#define SHAPE
 * If you want the Shaped window extensions, specify #define SHAPE
 *   Shaped window extensions seem to increase the window managers RSS
 *   by about 60 Kbytes. They provide for leaving a title-bar on the window
 *   without a border.
 *   If you dont use shaped window extension, you can either make your shaped
 *   windows undecorated, or live with a border and backdrop around all
 *   your shaped windows (oclock, xeyes)
 *
 *   If you normally use a shaped window (xeyes or oclock), you might as
 *   well compile this extension in, since the memory cost is  minimal in
 *   this case (The shaped window shared libs will be loaded anyway. If you
 *   don't normally use a shaped window, you have to decide for yourself
 
 *#define XPM
 *  if you want color icons, specify #define XPM, and get libXpm
 *  from sunsite.unc.edu. The following files are recommended in addition
 *  to the fvwm package (from sunsite.unc.edu):
 *   /pub/X11/contrib/xpm-3.2g.tar.Z for the Xpm library, or
 *   /pub/Linux/X11/libs, file libXpm32g.tar.z. for a pre-compiled shared 
 *                       library of libXpm for Linux.
 *   /pub/X11/contrib/xpm3icons.tar.Z, still more sample icons
 *   /pub/X11/contrib/ctwm-3.0.tar.Z, pull out the icons. ctwm has really nice
 *                                  color icons.
 *  and this from ftp.x.org:
 *   /contrib/icons.tar.gz, lots of sample icons,
 *
 *   For monochrome, Xpm icons still work, but they're only better than regular
 *   bitmaps because they're shaped (if you specify #define SHAPE).
 
 *#define HOTKEYS 
 *   adds support for keyboard shortcuts using the &
 *   prefix in the menu title and in the window-list
 *#define MULTIPLE_SCREENS
 *   Causes the crummy/broken multiple screen support to get compiled in
 *#define MODULES
 *   Causes support for modules/desktop accessories to be compiled in
 *#define M4
 *   Causes m4 pre-processor patches to be included. Try man m4 for more info.
 *   Warning: m4 defines macros for some simple things like "include"
 *            which might mess up a config like 
 *            IconPath /usr/include/X11/bitmaps, for example, so you
 *            would need to include
 *            undefine(`include') to fix that one. Some version of m4
 *            seem to give good error messages, others don't?
 *#define GNU_M4
 *   define this if you have GNU's m4
 *#define GNU_M4_OPTION_P
 *   define this if you have GNU's m4 (Version > 1.1) and 
 *   you dont want to mess up with simple things like include
 *   In this case option -P is given to m4, which means all
 *   builtins are prefixed with m4_. (See FvwmSaveDesk.man also)
 *#define SAVE_DESKTOP
 *   This option has been converted into a module.

 **************************************#
 * Memory Conservation
 **************************************#
 *#define NO_PAGER 
 *   Omits the code for the built-in pager. The pager module FvwmPager
 *   can be used instead.
 *#define NON_VIRTUAL
 *   Omits the virtual desktop - requires NO_PAGER
 *#define NO_ICONS
 *   Omits icon drawing (You have the use the window list)
 *#define NO_SAVEUNDERS 
 *   tells thw WM not to request save unders for pop-up
 *   menus. A quick test using monochrome X11 shows that save
 *   unders cost about 4Kbytes RAM, but saves a lot of
 *   window re#define raws if you have windows that take a while
 *   to refresh. For xcolor, I assume the cost is more like
 *   4Kbytesx8 = 32kbytes (256 color).
 *#define NO_WINDOWLIST 
 *   Caused fvwm built-in window-list to be omitted. The window-list
 *   module FvwmWinList can be used instead 
 *#define MENUCOLOR
 *   Allows menus to have a seperate color set from the windows.
 *#define PRUNE
 *   Removes old configuration commands:
 *       BoundaryWidth, NoBoundaryWidth, Sticky, NoTitle, NoBorder,
 *       StaysOnTop, StartsOnDesk, CirculateSkip, WindowListSkip, Icon,
 *       SuppressIcons, and Module (when used for initial startup
 *       These commands were replaced with Style, except for the Module
 *       command, which is replaced with InitFunction
 *       
 *
 **************************************#
 # User Preference
 **************************************#
 * These items are all run-time selectable now!
 
 **************************************#
 * System Library support
 **************************************#
 * 
 * Please refer to the file "documentation/OS_specifics" to find
 * settings of these parameters for your computer.
 *
 *#define NO_SYSCONF
 *   If your libraries don't have sysconf(), use this
 *#define NEEDS_STRNCASECMP
 *  If your libraries don't have strncasecmp(), use this
 *#define NEEDS_STRCASECMP            
 *  If your libraries don't have strcasecmp(), use this
 * #define NEEDS_ITIMERS               
 *  If your libraries don't have getitimer/setitimer, use this
 *  autoraise will not work, though.
 *#define NEEDS_SIGCHLD
 *  If you get zombies, especially from killing a module or leaving it
 *  up during a restart, use this
 *     SunOS-4.1.X NEEDS this
 *     Linux can go either way
 *     HP-UX DOES NOT want this, I think
 *     SCO DOES NOT want this
 *     Solaris DOES NOT want this
 *     others - try without for a few days, look for zombies, especially
 *              after a restart.
 *#define USE_SYSTEM
 *  Some people claim that they HAVE to use system() instead of
 *  execl() to do Exec's from within fvwm (or else they get Zombies).
 *  I personally think they should only need NEEDS_SIGCHLD, but who
 *  knows.
 *#define SYSV
 *  If you don't have wait3() or sigblock() calls use thi
 *#define NEEDS_MKSTEMP
 *  If you want m4 patches, and don't have mkstemp in your library, use this.
 *  (SCO Unix needs this)
 *#define UNION_WAIT
 *  If your system defines the first argument of wait3() to be a (union wait *)
 *  then uncomment this line. Default is (int *)
 *#define NEEDS_GETHOSTNAME
 *  Use this if you don't have gethostname().
 *
 ***************************************
 * Summary - Uncomment all your defs here! 
 ***************************************/
#define SHAPE                       
#define XPM                         
#define HOTKEYS                     
/* #define MULTIPLE_SCREENS            */
#define MODULES                     
/* #define M4                          */
/* #define GNU_M4                      */
/* #define GNU_M4_OPTION_P             */
#define MENUCOLOR                   
/* #define PRUNE                       */
#define NEW_STYLE_BUTTONS

/* #define NO_PAGER                    */
/* #define NON_VIRTUAL                 */
/* #define NO_ICONS                    */
/* #define NO_SAVEUNDERS               */
/* #define NO_WINDOWLIST               */

/* #define NO_SYSCONF                  */
/* #define NEEDS_STRNCASECMP           */
/* #define NEEDS_STRCASECMP            */
/* #define NEEDS_ITIMERS               */
/* #define NEEDS_MKSTEMP               */
#define NEEDS_SIGCHLD               
/* #define USE_SYSTEM                  */
/* #define SYSV                        */
/* #define BROKEN_SUN_HEADERS          */
/* #define UNION_WAIT                  */
/* #define NEEDS_GETHOSTNAME           */

/* Please translate the strings into the language which you use for your pop-up
 * menus */
/* Some decisions about where a function is prohibited (based on 
 * mwm-function-hints) is based on a string comparison between the 
 * menu item and the strings below */
#define MOVE_STRING "move"
#define RESIZE_STRING1 "size"
#define RESIZE_STRING2 "resize"
#define MINIMIZE_STRING "minimize"
#define MINIMIZE_STRING2 "iconify"
#define MAXIMIZE_STRING "maximize"
#define CLOSE_STRING1   "close"
#define CLOSE_STRING2   "delete"
#define CLOSE_STRING3   "destroy"
#define CLOSE_STRING4   "quit"

#ifdef __alpha
#define NEEDS_ALPHA_HEADER
#undef BROKEN_SUN_HEADERS
#endif /* (__alpha) */


/* Allows gcc users to use inline, doesn't cause problems
 * for others. */
#ifndef __GNUC__
#define  FVWM_INLINE  /*nothing*/
#else
#if defined(__GNUC__) && !defined(inline)
#define FVWM_INLINE __inline__
#else
#define FVWM_INLINE inline
#endif
#endif
