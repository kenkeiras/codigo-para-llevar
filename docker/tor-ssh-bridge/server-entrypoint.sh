#!/usr/bin/env bash

service ssh start
service tor start

watch -d -c -n 0.2 "echo '\033[1mServer side\033[0m';echo -n '  \033[1mHostname:\033[0m \033[7;32m'; cat /var/lib/tor/hidden_service/hostname; echo '\033[0m'; ifconfig -a"
