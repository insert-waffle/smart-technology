#!/bin/bash
# Setting the port speed
clear
ARDUINOPORT=/dev/ttyACM0

sudo stty -F $ARDUINOPORT raw speed 9600
sudo stty -F $ARDUINOPORT -hupcl

# login details for idrac server
IDRACIP="idrac-esx02.maes.local"
IDRACUSER="test"
IDRACPASS="test"

sleep 10s;
echo "ready for data"

while true;
do
        read result < $ARDUINOPORT
	EVAL=$(echo $result | cut -f1 -d"_")
        case $EVAL in
        S10xA)
                echo "Received DFC on"
		ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS raw 0x30 0x30 0x01 0x01;;
	S10xB)
		echo "Received DFC off, setting static speed"
		ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS raw 0x30 0x30 0x01 0x00
		ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS raw 0x30 0x30 0x02 0xff 0xA;;
	S1GET)
                echo "Asking for S1 status"
		T1=$(ipmitool -I lanplus -H $IDRACIP -U $IDRACUSER -P $IDRACPASS sdr type temperature | grep 0Eh | cut -d"|" -f5 | cut -d" " -f2)
		echo "CPU1$T1" > $ARDUINOPORT;;
        MAILTEMP)
		echo "Received temperature warning"
		echo "Temperature warning received. Please check your datacenter." | mail -s "[WARNING] Temperature" mail@ditisjens.be;;
	esac
done


