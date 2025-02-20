@echo off
cd \bootdsks.144\scsi-bat
echo "This option makes the qlog_isp.s disk."
echo "Please insert a formatted high-density diskette, and press"
echo "a key to make the disk, or CTRL-C to abort..."
pause
..\rawrite ..\qlog_isp.s a:
