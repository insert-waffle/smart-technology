#!/bin/bash
# Setting the port speed
ARDUINOPORT=/dev/ttyACM1

stty -F $ARDUINOPORT raw speed 9600

# login details for idrac server
IDRACIP="idrac-esx02.maes.local"
IDRACUSER="test"
IDRACPASS="test"

while true;
do
        read result < $ARDUINOPORT
	EVAL=$(echo $result | cut -f1 -d"_")
        case $EVAL in
        START)
                echo "Received start, setting fan speed to 0x32"
		ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS raw 0x30 0x30 0x02 0xff 0x32;;
	STOP)
		echo "Received stop, setting fan speed to 0xF"
		ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS raw 0x30 0x30 0x02 0xff 0xF;;
	*)
		echo "Oops...";;
        esac
done &

while true;
do
	T1=$(ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS sdr type temperature | grep 0Eh | cut -d"|" -f5 | cut -d" " -f2)
	T2=$(ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS sdr type temperature | grep 0Fh | cut -d"|" -f5 | cut -d" " -f2)
	echo "CPU1$T1" > $ARDUINOPORT
	sleep 5;
	echo "CPU2$T2" > $ARDUINOPORT
	sleep 10;
done
