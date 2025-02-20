@echo off
cd \bootdsks.12\scsi-bat
echo "This option makes the scsi.s disk."
echo "Please insert a formatted high-density diskette, and press"
echo "a key to make the disk, or CTRL-C to abort..."
pause
..\rawrite ..\scsi.s a:
