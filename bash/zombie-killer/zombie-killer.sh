#!/usr/bin/env bash
# Kills zombie process spawning from a parent PID

# check parameter number
if [ -z "$1" ];then
    echo "$0 <zombie parent pid>"
    exit 0
fi

# assign reasonable names to variables
ppid=$1
fname=`tempfile`

# GDB instructions
(for zpid in `ps h --ppid=$ppid|grep -P '^\s*\d+\s+[^ ]+\s+Z'|awk '{print $1;}'`;do
    echo "print waitpid($zpid, 0, 0)" # wait for each zombie
 done
 # and detach
 echo "detach") > $fname

# launch GDB
gdb -batch -p $ppid /proc/$ppid/exe -x $fname

# and cleanup
rm $fname

