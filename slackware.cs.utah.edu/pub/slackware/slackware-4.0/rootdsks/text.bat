@echo off
cd \rootdsks
echo "                                                                          "
echo "This option makes the TEXT rootdisk.  This is a text-based version of the "
echo "installation disk for Slackware Linux.                                    "
echo "                                                                          "
echo "Insert formatted disk in drive A: and hit a key to continue or CTRL-C     "
echo "to abort...                                                               "
echo "                                                                          "
pause

rawrite text.gz a:

