#!/bin/bash

serialport=$(zenity --entry --title "Enter path to serial port"--text "Which serial port is your tambourine thumper connected to?" --entry-text "/dev/ttyACM0"); echo $serialport
if test -z "$serialport" ; then
    exit 1
fi
thumps=$(zenity --scale --text "How many thumps" --min-value=1 --max-value=20 --value=3 --step 1)
if test -z "$thumps" ; then
    exit 2
fi
while (zenity --question --text="Thump tambourine $thumps times?") do
    echo $thumps
    for i in $(seq 1 $thumps) ; do
        echo "drums:*," > $serialport
        sleep 0.2
    done
done
