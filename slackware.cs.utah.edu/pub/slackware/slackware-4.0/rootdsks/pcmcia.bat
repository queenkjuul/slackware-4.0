@echo off
cd \rootdsks
echo "                                                                          "
echo "This option makes the PCMCIA rootdisk.  This is a floppy disk that allows "
echo "you to install Slackware Linux on laptop through a PCMCIA ethernet, SCSI, "
echo "or CD-ROM card to a Linux second extended filesystem.  You must have a    "
echo "partition free for this purpose.                                          "
echo "                                                                          "
echo "Insert formatted disk in drive A: and hit a key to continue or CTRL-C     "
echo "to abort...                                                               "
echo "                                                                          "
pause

rawrite pcmcia.gz a:

