@echo off
cd \bootdsks.144\ide-bat
echo "This option makes the xt.i disk."
echo "Please insert a formatted high-density diskette, and press"
echo "a key to make the disk, or CTRL-C to abort..."
pause
..\rawrite ..\xt.i a:
