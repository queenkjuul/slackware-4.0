Summary:     Kibble - a knowledge base program
Name:        kibble
Version:     0.7.1
Release:     3
Source:      kibble-0.7.1.tar.gz
Source1:     kibble-icon.png
Copyright:   GPL
Group:       einsetzen
BuildRoot:   /tmp/build
%description
This is Kibble, a knowledge base program. It is used to organize seemingly
discursive thoughts into a cohesive engine. Basically, it is used it to
keep track of random ideas that may prove useful.
%changelog

%prep

%setup

rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT

%build

./configure
make

%install

make install DESTDIR=$RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/share/pixmaps
cp $RPM_SOURCE_DIR/kibble-icon.png $RPM_BUILD_ROOT/usr/share/pixmaps

%clean
rm -rf $RPM_BUILD_ROOT

%files
%doc %attr(-,root,root) AUTHORS BUGS ChangeLog FAQ NEWS README THANKS TODO
%attr(-,root,root) /usr/local/bin/kibble
%attr(644,root,root) /usr/share/pixmaps/kibble-icon.png

# Local Variables: ***
# compile-command: "rpm -ba kibble.spec" ***
# End: ***