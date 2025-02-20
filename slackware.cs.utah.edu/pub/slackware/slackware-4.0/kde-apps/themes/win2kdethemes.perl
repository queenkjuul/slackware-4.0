#!/usr/bin/perl

# Devo

#   A script for converting Windows 95/98 desktop themes into KDE settings
#   Currently under heavy testing and revision.  Suggestions, improvements,
#   and bugfixes desperately wanted.

# Please send me mail if you like this program or think it can be improved.
#   Bill Garrett <garrett@circle-path.org>

# Copyright 1998 Bill Garrett; licensed under the GPL (GNU General Public
# License).  For more information, http://www.gnu.org/

&usage()
	unless	$THEME_zipfile = shift;

$THEME_tarfile = sprintf("%s.tar", $THEME_zipfile)
	unless	$THEME_tarfile = shift;

$THEME_temp = sprintf("/tmp/devo.%d", $$);
if( -d "$THEME_temp.in" || -d "$THEME_temp.out" ) {
	print "Error: temp directory already exists, something is wrong.\n";
	exit;
}

mkdir("$THEME_temp.in", 0755);
mkdir("$THEME_temp.out", 0755);
system("unzip $THEME_zipfile -d $THEME_temp.in > /dev/null");
$THEME_file = "";
if(opendir DIR, "$THEME_temp.in") {
	while($_ = readdir DIR) {
		if(/\.[Tt][Hh][Ee][Mm][Ee]$/) {
			$THEME_file = $_;
		}
	}
	closedir DIR;
} else {
	print "Couldn't open directory ", $THEME_temp, ".in\n";
	exit;
}

if($THEME_file eq "") {
	print "No theme file was found.\n";
	print "Either the zipfile doesn't exist, was corrupted, or wasn't ";
	print "what you expected it to be.\n";
	&safe_exit;
}

$THEME_name = substr($THEME_file, 0, length($THEME_file)-6);
print "Theme name: ", $THEME_name, "\n";
$THEME_rc = $THEME_name;
$THEME_rc =~ s/ /_/g;
$THEME_rc =~ s/[\(\)]//g;

%themedat = ();

if(! open THEME, "<$THEME_temp.in/$THEME_file") {
	print "Can't open theme file for some reason.\n";
	&safe_exit;
}
$lastkey = "";
while(<THEME>) {
	chop; chop;			# Stupid CRLF
	if(/^\[(.*)\]$/) {		# Start of a new keyspace
		$lastkey = $1;
	} elsif(/^[ \t]*$/) {		# Blank line
	} elsif(/^(.*)=(.*)$/) {	# key=value line
		$themedat{sprintf("%s.%s", $lastkey, $1)} = $2;
	}
}
close THEME;

print "Creating subdirectory structure...\n";
mkdir("$THEME_temp.out/share", 0755);
mkdir("$THEME_temp.out/share/apps", 0755);
mkdir("$THEME_temp.out/share/apps/kdisplay", 0755);
mkdir("$THEME_temp.out/share/apps/kdisplay/color-schemes", 0755);
mkdir("$THEME_temp.out/share/wallpapers", 0755);
mkdir("$THEME_temp.out/share/config", 0755);
mkdir("$THEME_temp.out/share/sounds", 0755);

print "Writing sound scheme as $THEME_rc.kwmsoundrc...\n";
print <<__END_OF_ANNOUNCE;
ATTENTION:
	Anyone who has a brilliant way of coping with the fact that kwmsoundrc
	expects hardcoded paths to the soundfiles is encouraged to email me.
	If your solution works well, I'll give you credit in the source
	code :)

For right now, you'll need to hand-edit the kwmsoundrc file we produce and
replace all instances of SOUND_DIR with the place your soundfiles are put.
The tarball has them in share/sounds/, but since we don't know the directory
you installed the tarball into that's the best we can do for now.
__END_OF_ANNOUNCE
open SOUNDFILE, ">$THEME_temp.out/share/config/$THEME_rc.kwmsoundrc";
print SOUNDFILE "# KDE Config File\n";
print SOUNDFILE "# No automatic configuration, sorry --\n";
print SOUNDFILE "# overwrite kwmsoundrc with this file and restart KDE\n";
print SOUNDFILE "# for changes to take effect.\n";
print SOUNDFILE "[GlobalConfiguration]\n";
print SOUNDFILE "EnableSounds=Yes\n";
print SOUNDFILE "[SoundConfiguration]\n";
&print_sound_file("Startup", "SystemStart");
&print_sound_file("LogoutMessage", "NO_EQUIVALENT");
&print_sound_file("Logout", "SystemExit");
&print_sound_file("WindowOpen", "Open");
&print_sound_file("WindowClose", "Close");
&print_sound_file("WindowResizeStart", "NO_EQUIVALENT");
&print_sound_file("WindowResizeEnd", "NO_EQUIVALENT");
&print_sound_file("WindowActivate", "blah");
&print_sound_file("WindowMoveStart", "blah");
&print_sound_file("WindowMoveEnd", "blah");
&print_sound_file("WindowShadeUp", "blah");
&print_sound_file("WindowShadeDown", "blah");
&print_sound_file("WindowMaximize", "Maximize");
&print_sound_file("WindowUnMaximize", "Minimize");
&print_sound_file("WindowSticky", "NO_EQUIVALENT");
&print_sound_file("WindowUnSticky", "NO_EQUIVALENT");
&print_sound_file("WindowIconify", "Minimize");
&print_sound_file("WindowDeIconify", "Maximize");
&print_sound_file("WindowTransNew", "NO_EQUIVALENT");
&print_sound_file("WindowTransDelete", "NO_EQUIVALENT");
&print_sound_file("Desktop1", "NO_EQUIVALENT");
&print_sound_file("Desktop2", "NO_EQUIVALENT");
&print_sound_file("Desktop3", "NO_EQUIVALENT");
&print_sound_file("Desktop4", "NO_EQUIVALENT");
&print_sound_file("Desktop5", "NO_EQUIVALENT");
&print_sound_file("Desktop6", "NO_EQUIVALENT");
&print_sound_file("Desktop7", "NO_EQUIVALENT");
&print_sound_file("Desktop8", "NO_EQUIVALENT");
close SOUNDFILE;

print "Writing background image as $THEME_rc.jpg...\n";
$wallfile = $themedat{"Control Panel\\Desktop.Wallpaper"};
$wallfile =~ s/%.*%//g;
system("convert", "$THEME_temp.in/$wallfile",
	"$THEME_temp.out/share/wallpapers/$THEME_rc.jpg");

print "Writing color scheme...\n";

open COLORFILE,
	">$THEME_temp.out/share/apps/kdisplay/color-schemes/$THEME_rc.kcsrc";
print COLORFILE "# KDE Config File\n";
print COLORFILE "[Color Scheme]\n";
print COLORFILE "name=", $THEME_name, "\n";
print COLORFILE "contrast=7\n";
&print_color_pattern("foreground", "WindowText");		# Most of these
&print_color_pattern("selectForeground", "ActiveTitle");	# are guesses
&print_color_pattern("activeBlend", "ActiveTitle");
&print_color_pattern("selectBackground", "Hilight");
&print_color_pattern("inactiveBackground", "InactiveBorder");
&print_color_pattern("background", "Background");
&print_color_pattern("activeBackground", "ActiveBorder");
&print_color_pattern("inactiveBlend", "InactiveTitle");
&print_color_pattern("inactiveForeground", "InactiveTitleText");
&print_color_pattern("activeForeground", "ActiveTitle");
&print_color_pattern("windowForeground", "WindowText");
&print_color_pattern("windowBackground", "WindowFrame");
close COLORFILE;

print "Cursors and desktop icons NOT YET SUPPORTED.\n";

print "Creating README file for soundtheme support...\n";
open README, ">$THEME_temp.out/share/sounds/README";
print README "ATTENTION:\n\n";
print README "In order to get support for the sounds included in your\n";
print README "Desktop Theme, you'll need to do two things.\n";

print README "1. Edit $THEME_rc.kwmsoundrc in .../share/config\n";
print README "   Replace all instances of SOUND_DIR with .../share/sounds, for example:\n";
print README "      Logout=SOUND_DIR/foo.wav\n\n";
print README "   becomes:\n";
print README "      Logout=/home/foo/.kde/share/sounds/foo.wav\n\n";
print README "   Any decent text editor should be able to do a global replace of this sort.\n\n";
print README "2. Rename $THEME_rc.kwmsoundrc to just kwmsoundrc\n";
print README "   Be sure to save your old kwmsoundrc file if you want to keep your current\n";
print README "   sound settings.  Hopefully KDE will incorporate a selector for sound\n";
print README "   themes in the near future.\n";
close README;

print "Creating tar file as $THEME_tarfile...\n";
system("tar -c -C $THEME_temp.out -f $THEME_tarfile .");

&safe_exit;

sub safe_exit {
	system("rm -rf $THEME_temp.in $THEME_temp.out");
	exit;
}

sub usage {
	print <<__END_OF_USAGE;
Usage: devo <zipfile> [<tarfile>]

	<zipfile> needs to be the full pathname of a ZIP file containing
	the Windows 95/98 desktop theme to convert.

	<tarfile> needs to be the full pathname of a TAR file you want to
	hold the KDE-compliant configuration files that devo produces.  If
	you do not specify a tarfile, the zipfile name will be appended with
	.tar.

	When you're ready to install your theme, just change directory to
	a KDE configuration directory (\$KDEDIR, /opt/kde, /usr/local/kde,
	\$HOME/.kde, etc.) and untar the tarball this script creates.
__END_OF_USAGE
	exit;
}

sub print_color_pattern {
	($kdename, $winname) = @_;

	$windata = $themedat{"Control Panel\\Colors.$winname"};
	$windata =~ s/ /,/g;
	print COLORFILE $kdename, "=", $windata, "\n";
}

sub print_sound_file {
	($kdename, $winname) = @_;

	$winsound = $themedat{"AppEvents\\Schemes\\Apps\\.Default\\$winname\\.Current.DefaultValue"};
	$winsound =~ s/%.*%//g;

	if($winsound eq "") {
		$winsound = "(none)";
	} else {
		# While we're here, make a copy of the soundfile
		$newsound = $winsound;
		$newsound =~ s/ /_/g;
		system("cp",
			"$THEME_temp.in/$winsound",
			"$THEME_temp.out/share/sounds/$newsound");
		$winsound = "SOUND_DIR/$newsound";
	}

	print SOUNDFILE $kdename, "=", $winsound, "\n";
}
