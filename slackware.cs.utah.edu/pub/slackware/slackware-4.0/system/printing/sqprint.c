From vax@sylvester.cc.utexas.edu Tue Oct  5 22:24:17 EDT 1993
Article: 2322 of comp.os.linux.misc
Path: samba.oit.unc.edu!concert!news-feed-2.peachnet.edu!darwin.sura.net!howland.reston.ans.net!math.ohio-state.edu!cs.utexas.edu!geraldo.cc.utexas.edu!sylvester.cc.utexas.edu!not-for-mail
From: vax@sylvester.cc.utexas.edu (Vax)
Newsgroups: comp.os.386bsd.misc,comp.os.linux.misc,comp.lang.perl
Subject: sqprint, a multi-page filter for HP Laserjets
Date: 5 Oct 1993 19:48:47 -0500
Organization: The University of Texas - Austin
Lines: 228
Message-ID: <28t4lf$1aa@sylvester.cc.utexas.edu>
NNTP-Posting-Host: sylvester.cc.utexas.edu
Keywords: PERL PCL LJ
Xref: samba.oit.unc.edu comp.os.386bsd.misc:1232 comp.os.linux.misc:2322 comp.lang.perl:19177

The following PERL script was written to print multiple pages in landscape
mode on a Hewlett-Packard Laserjet Series printer (PCL-lang only).
I conducted a search for other programs which would do this, but the search
was fruitless.  I hope the general community finds this program useful.

To use this program, you must install it in your printcap file.  I put mine
in the "cf" entry, for example, in my BSD-based system:

lp|local line printer:\
	:lp=/dev/lpa0:sd=/var/spool/lpd:lf=/var/log/lpd-errs:sh:\
	:if=/usr/foreign/bin/if_ljet3:\
	:cf=/usr/foreign/bin/sqprint:

The cf entry stands for "cif filter", or something like that.  Since I don't
have cif on my machine, I used the empty printcap slot for my filter.
However, if you install sqprint as a filter on your machine, be warned that
users who attempt to print cif files, or whatever filter you used, will have
quite a surprise in store for them.

To print on my system, I use:
	lpr -c inputfilename

The "-c" tells lpd to print the file using the cif output filter.

Unfortunately, very few useful arguments (such as input filename, etc) are
passed to any filters except "if" or "of", both of which you will probably
not want to override.  If you do override the "if" or "of" filters, you will
have to change the print_footer routine at the end of the file.

There is no guarantee of support from me, although I will probably continue
to add features as I find them useful.  I never intend to make a MS-DOS port.

I will not answer any mail regarding "how to install this program" or
"the program won't work with my HP calculator, what is wrong?".  If you have
problems, you will have to resolve them yourself.

Check you printcap and lpr manpages for more info.

Mail kudos, remarks, subpoenas to vax@ccwf.cc.utexas.edu.

----------If you cut here you will damage your monitor------8<-----------------
#!/usr/bin/perl
# sqprint v1.0
# Written by vax@ccwf.cc.utexas.edu (VaX#n8) October 4th, 1993
# This program may not be used in any commercial product without written
# permission of the author.
# This program is freely redistributable (with the exception noted above)
# as long as this header remains with the file.

$line=-1;
$col=-1;
while (<STDIN>)
{
	chop;
	$temp = $_;
	while (($l = length($temp)) > 0)
	{
		$_ = substr($temp,0,($l < 80) ? $l : 80);
		$temp = substr($temp,length($_));
		$line = ($line + 1) % 65;
		if ($line == 0)
		{
			if ($col == 1)
			{
				# send form feed (FF)
				print "\x0C";
			}
			$col = ($col + 1) % 2;
			if ($col == 0)
			{
				&setup_page;
			}
			else
			{
				print "\x1B&k7.2029H";
				print "\x1B&a91lhV";
				print "\n";
			}
			&setup_font_stuff;
			&setup_column;
		}
		print ;
		print "\n";
	}
}

# send form feed (FF)
print "\x0C";

# reset printer
print "\x1BE";


sub setup_font_stuff
{
	
	# set font type to PC8
	print "\x1B(10U";
	
	# set default [non]proportional font (?)
	# 16.6 CPI pitch
	# 8.5 point vertical
	# set default upright/italic
	# set default stroke weight
	# set default typeface
	print "\x1B(sp16.6h8.5vsbT";
}

sub draw_box
{
	# horizonal move to 155 decipoints
	# vertical move to 452 decipoints
	print "\x1B&a155h452V";
	
	# create a graphic element 7325 horizontal decipoints
	# create a graphic element 1 vertical decipoint
	# draw
	print "\x1B*c7325h1vP";
	
	# horizontal move to 155 decipoints
	# vertical move to 5652 decipoints
	print "\x1B&a155h5652V";
	
	# same as two above
	print "\x1B*c7325h1vP";
	
	# move to (155,452)
	print "\x1B&a155h452V";
	
	# make a vertical line 5200 decipoints high, 1 wide 
	print "\x1B*c1h5200vP";
	
	# move to (3830,452)
	print "\x1B&a3830h452V";
	
	# same as two above
	print "\x1B*c1h5200vP";
	
	# move to (7480,452)
	print "\x1B&a7480h452V";
	
	# make another line 1 wide, 5200 high
	print "\x1B*c1h5200vP";
	
	# move to (7524,5352)
	print "\x1B&a7524h5352V";
}

sub setup_page
{
	# reset printer
	print "\x1BE";
	
	# print in landscape mode
	print "\x1B&l1O";

	&setup_font_stuff;
	
	# set column width (HMI) to 7.2/120ths of an inch
	print "\x1B&k7.2H";
		
	# set VMI to 5.2/42ths of an inch
	# set default perforation skip
	print "\x1B&l5.2cL";
	
	# set default number of lines in top margin
	print "\x1B&lE";
		
	# set default left margin
	print "\x1B&aL";
	
	&draw_box;

	&setup_font_stuff;
	
	# set HMI to 7.2029/120ths of an inch
	print "\x1B&k7.2029H";
	
	# set VMI to 15.2/42ths of an inch
	# set default perforation skip
	print "\x1B&l5.2cL";
	
	# set line termination to cr->cr; lf->cr+lf; ff->cr+ff
	print "\x1B&k2G";

	&print_footer;

	# horizontal move -5 decipoints,
	# vertical move +3 decipoints
	print "\x1B&a-5h+3V";
	
	# Set top margin 7 lines
	print "\x1B&l7E";
	
	print "\x1B&a6lhV";
	print "\n";
	
}

sub setup_column
{
	# set column width (HMI) to 7.2/120ths of an inch
	print "\x1B&k7.2H";
	
	# set VMI to 05.200/42ths of an inch
	print "\x1B&l05.200C";
}

sub print_footer
{
	# args are of form  -xwidth -ylength -n login -h hostname acct-file

	# move to (155,5652) decipoints
	print "\x1B&a155h5652V";

	print "\n";

	# 172 or more spaces for footer
	print "    login: $ARGV[3]\thostname: $ARGV[5]\n";
	
	print "\x1B&a+5h-3V";
	
	# an emply line that's left open for later stuff
	print "\n";
}
-- 
Protect our endangered bandwidth - reply by email.  NO BIG SIGS!
VaX#n8 vax@ccwf.cc.utexas.edu - Don't blame me if the finger daemon is down


