#!/bin/sh

set -eu

# Move to directory where this is executed

cd $(dirname "$0")

source params.bash

while [ 1 ];do
	websocat -U "wss://${DOMAIN}/stream?token=${TOKEN}"|xargs -d$'\n' -n1 sh notify.sh
	sleep 10
done
