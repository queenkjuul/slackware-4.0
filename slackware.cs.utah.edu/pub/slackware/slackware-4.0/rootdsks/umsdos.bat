@echo off
cd \rootdsks
echo "                                                                          "
echo "This option makes the UMSDOS rootdisk.  This is a floppy                  "
echo "disk that allows you to install Slackware Linux on an existing DOS        "
echo "partition in the \LINUX directory.  (If you wish to install Linux using   "
echo "Linux's high-performance second extended filesystem, you will need to     "
echo "use the COLOR rootdisk instead.)                                          "
echo "                                                                          "
echo "Insert formatted disk in drive A: and hit a key to continue or CTRL-C     "
echo "to abort...                                                               "
echo "                                                                          "
pause

rawrite umsdos.gz a:

