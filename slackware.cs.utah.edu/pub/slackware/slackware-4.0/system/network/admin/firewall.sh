#!/bin/bash
# firewall.sh v1.0
# This script is copyright by Patrick Lambert <drow@darkelf.net>
#
# Redistribution and use of this script, with or without modification, is
# permitted provided that the following conditions are met:
#
# 1. Redistributions of this script must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR `AS IS'' AND ANY EXPRESS OR IMPLIED
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
IPFWADM=/sbin/ipfwadm
RCSCRIPT=rc.firewall
TITLE="Firewall utility for IPFW (Linux kernel 2.0)"

reset
rm -f $TMP/.firewall*

menu()
{
$DIALOG --title "$TITLE" --menu \
"Type of rules to input. Note that you should be root to do this \
and have firewalling compiled in your kernel. I'll be using $TMP as temp, \
$IPFWADM as firewall interface, and every rule you want \
will be appended to $RCSCRIPT" 20 75 8 "A" "Accounting rules" \
"I" "Firewall input rules" "O" "Firewall output rules" "F" "Forwarding \
rules" "M" "Masquerading rules" "P" "Pre-defined rules" "H" "Help" "X" \
"Exit firewall.sh" 2> $TMP/.firewall

util

if [ "$CHOICE" = "I" ]; then
 input
fi

if [ "$CHOICE" = "O" ]; then
 output
fi

if [ "$CHOICE" = "A" ]; then
 accounting
fi

if [ "$CHOICE" = "F" ]; then
 forward
fi

if [ "$CHOICE" = "M" ]; then
 masq
fi

if [ "$CHOICE" = "P" ]; then
 predefined
fi

if [ "$CHOICE" = "H" ]; then
 help
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
CHOICE="`cat $TMP/.firewall`"
}

util2()
{
if [ $? = 1 -o $? = 255 ]; then
 menu
fi
CHOICE="`cat $TMP/.firewall`"
}

help()
{
dialog --sleep 10 --title "$TITLE" \
--infobox "\nThis utility helps you pick firewalling choices and\ncreate a \
script ($RCSCRIPT) with those rules,\nusing $IPFWADM\n\nThis program is (C) \
1999 Patrick Lambert <drow@darkelf.net>\nand provided AS-IS with no warranty. \
\nEdit this file for more." 15 75
menu
}

predefined()
{
$DIALOG --title "$TITLE" --menu "Which one should I add" \
12 75 4 "1" "Clear current and set default to accept" "2" "Some input \
firewall rules" "3" "System defined rules" \
"4" "Default masquerading rules" 2>$TMP/.firewall
util2

if [ "$CHOICE" = "1" ]; then
 RULE="$IPFWADM -I -f"
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
 RULE="$IPFWADM -I -p accept"        
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
 RULE="$IPFWADM -O -f"
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
 RULE="$IPFWADM -O -p accept"        
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
 RULE="$IPFWADM -F -f"        
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
 RULE="$IPFWADM -F -p accept"        
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
fi
if [ "$CHOICE" = "2" ]; then
 dest
 iface
 RULE="$IPFWADM -I -I -a deny $IFACE -S $DEST -D $DEST"
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
 $DIALOG --sleep 5 --title "$TITLE" --infobox "The rule added will deny \
 every packet comming from $DEST to $DEST on the specified interface, which \
 would mean spoof packets" 20 75
fi
if [ "$CHOICE" = "3" ]; then
 $DIALOG --sleep 5 --title "$TITLE" --infobox "This option is here so the \
 system admin can create a file with specified rules, and you can append this \
 to your current firewall" 20 75
 $DIALOG --title "$TITLE" --inputbox "Enter the name of the file to be \
 appended" 10 75  2>$TMP/.firewall
 util2
 cat "$CHOICE" >> $RCSCRIPT
fi
if [ "$CHOICE" = "4" ]; then
 $DIALOG --title "$TITLE" --inputbox "Enter the IP/netmask of the network \
 you want to use your gateway (ie: 192.168.1.0/24)" 10 75 2>$TMP/.firewall
 util2
 RULE="$IPFWADM -F -p deny"
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
 RULE="$IPFWADM -F -a m -S $CHOICE -D 0.0.0.0/0"
 $DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
 echo "$RULE" >> $RCSCRIPT
fi
menu
}

masq()
{
$DIALOG --title "$TITLE" --inputbox "Enter the 3 timeouts for TCP TCPfin and \
UDP packets when using IPMASQ (enter 0 0 0 to keep current values)" \
8 75 2>$TMP/.firewall
util2
RULE="$IPFWADM -M -s $CHOICE"
$DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
echo "$RULE" >> $RCSCRIPT
menu
}

input()
{
rule
if [ "$CHOICE" = "default" ]; then
 DEFAULT="-I"
 default
 menu
 exit
fi
multi
RULE="$IPFWADM -I $TYPE $TRAF -P $PROTO $IFACE -S $SOURCE -D $DEST $PORT"
$DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
echo "$RULE" >> $RCSCRIPT
menu
}

forward()
{
rule
if [ "$CHOICE" = "default" ]; then
 DEFAULT="-F"
 default
 menu
 exit
fi
multi
RULE="$IPFWADM -F $TYPE $TRAF -P $PROTO $IFACE -S $SOURCE -D $DEST $PORT"
$DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
echo "$RULE" >> $RCSCRIPT
menu
}

accounting()
{
multi
RULE="$IPFWADM -A $TYPE $TRAF -P $PROTO $IFACE -S $SOURCE -D $DEST $PORT"
$DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
echo "$RULE" >> $RCSCRIPT
menu
}

output()
{
rule
if [ "$CHOICE" = "default" ]; then
 DEFAULT="-O"
 default
 menu
 exit
fi
multi
RULE="$IPFWADM -O $TYPE $TRAF -P $PROTO $IFACE -S $SOURCE -D $DEST $PORT"
$DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
echo "$RULE" >> $RCSCRIPT
menu
}

default()
{
traf
RULE="$IPFWADM $DEFAULT -p $TRAF"
$DIALOG --sleep 2 --title "$TITLE" --infobox "Adding: $RULE to $RCSCRIPT" 10 75
echo "$RULE" >> $RCSCRIPT
menu
}

multi()
{
type
traf
proto
source
dest
port
iface
}

rule()
{
$DIALOG --title "$TITLE" --menu "What do you want to do" \
10 75 2 "add" "Add a new rule" "default" "Change the default" 2>$TMP/.firewall
util2
}

type()
{
$DIALOG --title "$TITLE" --menu "What to do with this rule" 10 75 3 \
"insert" "Insert at beginning" "append" "Append at the end" "delete" "Delete \
this rule" 2>$TMP/.firewall
util2

if [ "$CHOICE" = "append" ]; then
 TYPE="-a"
fi

if [ "$CHOICE" = "insert" ]; then
 TYPE="-i"
fi

if [ "$CHOICE" = "delete" ]; then
 TYPE="-d"
fi
}

traf()
{
$DIALOG --title "$TITLE" --menu "This rule will be to allow or deny traffic" \
12 75 4 "accept" "Accept traffic" "deny" "Deny traffic" "reject" "Reject \
traffic" "masquerade" "Masquerade traffic" 2>$TMP/.firewall
util2
TRAF=$CHOICE
}

proto()
{
$DIALOG --title "$TITLE" --menu "Select a protocol for this rule" 12 75 4 \
"tcp" "TCP protocol" "udp" "UDP protocol" "icmp" "ICMP protocol" \
"all" "All protocols" 2>$TMP/.firewall
util2
PROTO=$CHOICE
}

source()
{
$DIALOG --title "$TITLE" --inputbox "Enter the source IP/netmask (ie: \
192.168.196.10/32)" 8 75 2>$TMP/.firewall
util2
SOURCE=$CHOICE
}

dest()
{
$DIALOG --title "$TITLE" --inputbox "Enter the destination (usualy your \
system) IP/netmask (ie: 28.56.123.0/24)" 8 75 2>$TMP/.firewall
util2
DEST=$CHOICE
}

port()
{
$DIALOG --title "$TITLE" --inputbox "Port number (enter nothing for all \
ports)" 8 75 2>$TMP/.firewall
util2
PORT=$CHOICE
}

iface()
{
$DIALOG --title "$TITLE" --inputbox "Interface (enter nothing for all \
interfaces, ie: eth0)" 8 75 2>$TMP/.firewall
util2
IFACE=""
if [ "$CHOICE" != "" ]; then
 IFACE="-W $CHOICE"
fi
}

menu
