@echo off
cd \bootdsks.144\scsi-bat
echo "This option makes the sym538xx.s disk."
echo "Please insert a formatted high-density diskette, and press"
echo "a key to make the disk, or CTRL-C to abort..."
pause
..\rawrite ..\sym538xx.s a:
