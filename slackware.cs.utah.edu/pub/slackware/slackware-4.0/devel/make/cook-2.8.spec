Summary: a file construction tool
Name: cook
Version: 2.8
Release: 1
Copyright: GPL
Group: Development/Building
Source: http://www.canb.auug.org.au/~millerp/cook-2.8.tar.gz
URL: http://www.canb.auug.org.au/~millerp/cook.html
BuildRoot: /tmp/cook-build-root
Icon: cook.gif

%description
Cook is a tool for constructing files. It is given a set of files to
create, and recipes of how to create them. In any non-trivial program
there will be prerequisites to performing the actions necessary to
creating any file, such as include files.  The cook program provides a
mechanism to define these.

When a program is being developed or maintained, the programmer will
typically change one file of several which comprise the program.  Cook
examines the last-modified times of the files to see when the
prerequisites of a file have changed, implying that the file needs to be
recreated as it is logically out of date.

Cook also provides a facility for implicit recipes, allowing users to
specify how to form a file with a given suffix from a file with a
different suffix.  For example, to create filename.o from filename.c

* Cook has a simple but powerful string-based description language with
  many built-in functions.  This allows sophisticated filename
  specification and manipulation without loss of readability or
  performance.

* Cook is able to use fingerprints to supplement file modification
  times.  This allows build optimization without contorted rules.

* Cook is able to build your project with multiple parallel threads,
  with support for rules which must be single threaded.  It is possible
  to distribute parallel builds over your LAN, allowing you to turn your
  network into a virtual parallel build engine.

%package psdocs
Summary: Cook documentation, PostScript format
Group: Development/Building

%description psdocs
Cook documentation in PostScript format.

%package dvidocs
Summary: Cook documentation, DVI format
Group: Development/Building

%description dvidocs
Cook documentation in DVI format.

%prep
%setup
./configure --prefix=/usr

%build
make

%install
make RPM_BUILD_ROOT=$RPM_BUILD_ROOT install

%files
%doc /usr/share/cook/en/refman.txt
%doc /usr/share/cook/en/user-guide.txt
/usr/bin/c_incl
/usr/bin/cook
/usr/bin/cook_bom
/usr/bin/cookfp
/usr/bin/cooktime
/usr/bin/find_libs
/usr/bin/make2cook
/usr/bin/roffpp
/usr/lib/cook/en/LC_MESSAGES/c_incl.mo
/usr/lib/cook/en/LC_MESSAGES/cook.mo
/usr/lib/cook/en/LC_MESSAGES/cook_bom.mo
/usr/lib/cook/en/LC_MESSAGES/cookfp.mo
/usr/lib/cook/en/LC_MESSAGES/cooktime.mo
/usr/lib/cook/en/LC_MESSAGES/find_libs.mo
/usr/lib/cook/en/LC_MESSAGES/make2cook.mo
/usr/lib/cook/en/LC_MESSAGES/roffpp.mo
/usr/man/man1/c_incl.1
/usr/man/man1/cook.1
/usr/man/man1/cook_bom.1
/usr/man/man1/cook_lic.1
/usr/man/man1/cookfp.1
/usr/man/man1/cooktime.1
/usr/man/man1/find_libs.1
/usr/man/man1/make2cook.1
/usr/man/man1/roffpp.1
/usr/share/cook/as
/usr/share/cook/bison
/usr/share/cook/c
/usr/share/cook/en/man1/c_incl.1
/usr/share/cook/en/man1/cook.1
/usr/share/cook/en/man1/cook_bom.1
/usr/share/cook/en/man1/cook_lic.1
/usr/share/cook/en/man1/cookfp.1
/usr/share/cook/en/man1/cooktime.1
/usr/share/cook/en/man1/find_libs.1
/usr/share/cook/en/man1/make2cook.1
/usr/share/cook/en/man1/roffpp.1
/usr/share/cook/f77
/usr/share/cook/functions
/usr/share/cook/g77
/usr/share/cook/gcc
/usr/share/cook/home
/usr/share/cook/lex
/usr/share/cook/library
/usr/share/cook/print
/usr/share/cook/program
/usr/share/cook/rcs
/usr/share/cook/recursive
/usr/share/cook/sccs
/usr/share/cook/text
/usr/share/cook/usr
/usr/share/cook/usr.local
/usr/share/cook/yacc
/usr/share/cook/yacc_many

%files psdocs
/usr/share/cook/en/refman.ps
/usr/share/cook/en/user-guide.ps

%files dvidocs
/usr/share/cook/en/refman.dvi
/usr/share/cook/en/user-guide.dvi
