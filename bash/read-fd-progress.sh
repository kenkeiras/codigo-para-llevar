#!/usr/bin/env bash

# Check that we have at least a PID
if [ -z "$1" ];then
   echo "$0 <PID> [<FD>]"
   exit 1
fi

set -eu  # Bail on any error
PID=$1
FD=${2:-} # Take FD if we have second parameter, else consider it empty

if [ -z "$FD" ];then
   # Show the user the available file descriptors
   echo "Select a file descriptor:"
   for i in /proc/$PID/fd/*;do
       printf "  %s: " $(basename $i)
       readlink $i
   done

   read -p "FD: " FD
fi

FSIZE=$(stat -L /proc/$PID/fd/$FD --printf=%s)  # Read full file size
while [ 1 ];do

      # Stop if the process has finished reading the file
      if [ ! -f /proc/$PID/fdinfo/$FD ];then
         break
      fi

      # Read position on file
      x=$(cat "/proc/$PID/fdinfo/$FD"|grep pos: |cut -d: -f2|tr -d '\t ')

      # Convert that position into a % of the file size
      PER10000=$(( $x * 10000 / $FSIZE ))
      if [ $PER10000 -le 100 ];then
         # Less than 1%
         printf "  0.%02i%%\n" $(( $PER10000 ));
      else
         # More than 1%
         printf "%3i.%02i%%\n" $(( $PER10000 / 100 )) $(( $PER10000 % 100 ));
      fi

      # Wait for the next loop
      sleep 1
done
