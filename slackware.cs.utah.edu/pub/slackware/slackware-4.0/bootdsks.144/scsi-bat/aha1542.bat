@echo off
cd \bootdsks.144\scsi-bat
echo "This option makes the aha1542.s disk."
echo "Please insert a formatted high-density diskette, and press"
echo "a key to make the disk, or CTRL-C to abort..."
pause
..\rawrite ..\aha1542.s a:
