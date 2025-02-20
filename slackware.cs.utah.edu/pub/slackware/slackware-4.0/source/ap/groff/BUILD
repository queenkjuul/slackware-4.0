
Here's how the Slackware groff package is built:

tar xzvf groff-1.11a.tar.gz
cd groff-1.11
zcat groff-1.11a.xditview.diff.gz | patch
configure --prefix=/usr
make CFLAGS=-O2 CCFLAGS=-O2
