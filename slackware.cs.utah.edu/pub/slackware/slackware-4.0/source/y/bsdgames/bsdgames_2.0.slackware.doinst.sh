#!/bin/sh
if fgrep fortune etc/profile ; then
	BOGUS_FLAG="bogus_value"
else
        echo "# Print a fortune cookie for login shells:" >> etc/profile
	echo "echo" >> etc/profile
	echo "fortune /usr/games/lib/fortunes/fortunes /usr/games/lib/fortunes/fortunes2" >> etc/profile
	echo "echo" >> etc/profile
fi
if [ -r etc/csh.login ]; then 
	if fgrep fortune etc/csh.login ; then
		BOGUS_FLAG="bogus_value"
	else
        	echo "# Print a fortune cookie for login shells:" >> etc/csh.login
		echo 'if ( { tty --silent } ) then >& /dev/null' >> etc/csh.login
		echo '  echo "" ; fortune /usr/games/lib/fortunes/fortunes /usr/games/lib/fortunes/fortunes2 ; echo ""' >> etc/csh.login
		echo 'endif' >> etc/csh.login
	fi
fi
