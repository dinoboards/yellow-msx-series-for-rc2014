#!/usr/bin/env bash

# Get the value of the symbol name suppied

val=$(grep "$2" "$1" | cut -d ' ' -f3)
val=${val:2}

if [ "$val" == "" ]; then
  echo "Symbol not found"
  exit 1
fi

printf "%d" $((16#$val))
