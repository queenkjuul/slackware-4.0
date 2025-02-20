Summary: file history and comparison tools
Name: fhist
Version: 1.4
Release: 1
Copyright: GPL
Group: Development/Tools
Source: http://www.canb.auug.org.au/~millerp/fhist-1.4.tar.gz
URL: http://www.canb.auug.org.au/~millerp/fhist.html
BuildRoot: /tmp/fhist-build-root

%description
The FHist package contains 3 utilities, a file history tool ``fhist'',
a file comparison tool ``fcomp'', and a file merging tool ``fmerge''.
All three are bundled together, because they all use the same
minimal-difference algorithm.

The history tool presented here, fhist, is a minimal history tool.
It provides no locking or branching.  This can be useful in contexts
where the configuration management or change control be being provided
by some other tool.

%prep
%setup

%build
./configure --prefix=/usr
make

%install
make RPM_BUILD_ROOT=$RPM_BUILD_ROOT install

%files
/usr/bin/fcomp
/usr/bin/fhist
/usr/bin/fmerge
/usr/lib/fhist/en/LC_MESSAGES/fcomp.mo
/usr/lib/fhist/en/LC_MESSAGES/fhist.mo
/usr/lib/fhist/en/LC_MESSAGES/fmerge.mo
/usr/man/man1/fcomp.1
/usr/man/man1/fhist.1
/usr/man/man1/fhist_lic.1
/usr/man/man1/fmerge.1
/usr/share/fhist/en/building.dvi
/usr/share/fhist/en/building.ps
/usr/share/fhist/en/building.txt
/usr/share/fhist/en/man1/fcomp.1
/usr/share/fhist/en/man1/fhist.1
/usr/share/fhist/en/man1/fhist_lic.1
/usr/share/fhist/en/man1/fmerge.1
/usr/share/fhist/en/readme.dvi
/usr/share/fhist/en/readme.ps
/usr/share/fhist/en/readme.txt
/usr/share/fhist/en/reference.dvi
/usr/share/fhist/en/reference.ps
/usr/share/fhist/en/reference.txt

%clean
rm -rf $RPM_BUILD_ROOT
