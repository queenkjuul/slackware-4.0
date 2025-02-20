@echo off
cd \bootdsks.144\ide-bat
echo "This option makes the sanyo.i disk."
echo "Please insert a formatted high-density diskette, and press"
echo "a key to make the disk, or CTRL-C to abort..."
pause
..\rawrite ..\sanyo.i a:
