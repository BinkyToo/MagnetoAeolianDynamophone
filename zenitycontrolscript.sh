#!/bin/bash
# This script provites a simple gui interface for triggering a hardware drum-beater over a (virtual) serial port
# Originally written to go with an Arduino running "MADdrumslave"

serialport=$(zenity --entry --title "Enter path to serial port"--text "Which serial port is your tambourine thumper connected to?" --entry-text "/dev/ttyACM0"); echo $serialport
if test -z "$serialport" ; then #Quit if the user pressed cancel
    exit 1
fi
thumps=$(zenity --scale --text "How many thumps" --min-value=1 --max-value=20 --value=3 --step 1)
if test -z "$thumps" ; then     #Quit if the user pressed cancel
    exit 2
fi
while (zenity --question --text="Thump tambourine $thumps times?") do
    for i in $(seq 1 $thumps) ; do  #repeatedly trigger a drum thump
        echo "percussion:*,\n" > $serialport   #send command string over serial connection
        if [ $? = 1 ] ; then
            zenity --error --text "Could not send command to serial device.\nPlease ensure that you have selected the correct serial port\nand that it is not being used by another program."
            exit 3
        fi
        sleep 3                       #FIXME this shouldn't be necessary
    done
done
