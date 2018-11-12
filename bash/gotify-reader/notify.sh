#!/usr/bin/env bash

echo "$1"

title=$(echo "$1" | jq -r .title)
message=$(echo "$1" | jq -r .message)

notify-send "$title" "$message"
