#!/bin/bash
# proc.sh v1.0
# This script is copyright by Patrick Lambert <drow@darkelf.net>
#
# Redistribution and use of this script, with or without modification, is
# permitted provided that the following conditions are met:
#
# 1. Redistributions of this script must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR S IS'' AND ANY EXPRESS OR IMPLIED
#  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
#  EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Layout for this script was based on Slackware's /sbin/setup
#
TMP=/tmp
DIALOG=dialog
TITLE="Proc System Information utility"

reset
rm -f $TMP/.firewall*

menu()
{
$DIALOG --title "$TITLE" --menu "Pick a selection" 20 75 5 "1" "Devices" "2" \
"Files" "3" "Kernel" "4" "Misc" "X" "Exit" 2> $TMP/.proc
util
if [ "$CHOICE" = "1" ]; then
 devices
fi
if [ "$CHOICE" = "2" ]; then
 files
fi
if [ "$CHOICE" = "3" ]; then
 kernel
fi
if [ "$CHOICE" = "4" ]; then
 misc
fi
if [ "$CHOICE" = "X" ]; then
 reset
 clear
 exit
fi
}

util()
{
if [ $? = 1 -o $? = 255 ]; then
 exit
fi
CHOICE="`cat $TMP/.proc`"
}

util2()
{
if [ $? = 1 -o $? = 255 ]; then
 menu
fi
CHOICE="`cat $TMP/.proc`"
}

devices()
{
$DIALOG --title "$TITLE" --menu "Pick a selection" 20 75 7 "cpuinfo" "CPU \
information" "meminfo" "System memory" "devices" "Detected devices" \
"interrupts" "Interrupts of devices" "ioports" "I/O addresses of devices" \
"dma" "DMA addresses of devices" "pci" "Detected PCI devices" 2> $TMP/.proc
util2
$DIALOG --title "$TITLE" --textbox "/proc/$CHOICE" 20 75
menu
}

files()
{
$DIALOG --title "$TITLE" --menu "Pick a selection" 20 75 4 "filesystems" \
"Supported file systems" "partitions" "Disk partitions" "mounts" "Current \
mounted partitions" "locks" "Locked files" 2> $TMP/.proc
util2
$DIALOG --title "$TITLE" --textbox "/proc/$CHOICE" 20 75
menu
}

kernel()
{
$DIALOG --title "$TITLE" --menu "Pick a selection" 20 75 5 "version" \
"Current kernel version" "loadavg" "Load average of the system" "uptime" \
"Current system uptime" "modules" "Loaded kernel modules" "cmdline" "Kernel \
boot line" 2> $TMP/.proc
util2
$DIALOG --title "$TITLE" --textbox "/proc/$CHOICE" 20 75
menu
}

misc()
{
$DIALOG --title "$TITLE" --menu "Pick a selection" 20 75 2 "rtc" "Real-time \
clock information" "misc" "Misc devices" 2> $TMP/.proc
util2
$DIALOG --title "$TITLE" --textbox "/proc/$CHOICE" 20 50
menu
}

menu

