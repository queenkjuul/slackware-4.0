@echo off
cd \rootdsks
echo "                                                                          "
echo "This option makes the RESCUE rootdisk.  This is a floppy                  "
echo "disk with a small Linux system that has an editor (vi), lilo, e2fsck,     "
echo "and other programs useful for fixing your machine if you ever get locked  "
echo "out for some reason.                                                      "
echo "                                                                          "
echo "Insert formatted disk in drive A: and hit a key to continue or CTRL-C     "
echo "to abort...                                                               "
echo "                                                                          "
pause

rawrite rescue.gz a:

