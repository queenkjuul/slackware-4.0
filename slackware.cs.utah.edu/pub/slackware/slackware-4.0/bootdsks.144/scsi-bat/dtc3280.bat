@echo off
cd \bootdsks.144\scsi-bat
echo "This option makes the dtc3280.s disk."
echo "Please insert a formatted high-density diskette, and press"
echo "a key to make the disk, or CTRL-C to abort..."
pause
..\rawrite ..\dtc3280.s a:
