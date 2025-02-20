@echo off
cd \rootdsks
echo "                                                                          "
echo "This option makes the COLOR rootdisk.  This is a floppy                   "
echo "disk that allows you to install Slackware Linux on a partition using the  "
echo "Linux second extended filesystem.  You must have a partition available for"
echo "this purpose.  (If you wish to install Linux on an existing MS-DOS        "
echo "partition, please use the UMSDOS disk instead.)                           "
echo "                                                                          "
echo "Insert formatted disk in drive A: and hit a key to continue or CTRL-C     "
echo "to abort...                                                               "
echo "                                                                          "
pause

rawrite color.gz a:

